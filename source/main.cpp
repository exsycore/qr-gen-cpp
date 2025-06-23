#include "main.hpp"

using namespace qrcodegen;

std::string generate_qr_png_memory(const std::string &text)
{
    using namespace qrcodegen;

    QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);
    const int scale = 10;
    const int border = 4;
    const int size = (qr.getSize() + border * 2) * scale;

    // เตรียม buffer ข้อมูล PNG
    std::vector<unsigned char> png_buffer;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // ตั้ง write callback เขียนลง vector
    png_set_write_fn(png_ptr, &png_buffer, [](png_structp png_ptr, png_bytep data, png_size_t length)
    {
        auto *out = reinterpret_cast<std::vector<unsigned char>*>(png_get_io_ptr(png_ptr));
        out->insert(out->end(), data, data + length);
    }, nullptr);

    png_set_IHDR(png_ptr, info_ptr, size, size,
        8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // เขียน pixel line-by-line
    std::vector<png_byte> row(size);
    for (int y = -border; y < qr.getSize() + border; y++)
    {
        for (int sy = 0; sy < scale; sy++)
        {
            for (int x = -border; x < qr.getSize() + border; x++)
            {
                bool pixel = qr.getModule(x, y);
                unsigned char color = pixel ? 0 : 255;
                for (int sx = 0; sx < scale; sx++)
                    row[(x + border) * scale + sx] = color;
            }
            png_write_row(png_ptr, row.data());
        }
    }

    png_write_end(png_ptr, nullptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return std::string(reinterpret_cast<const char*>(png_buffer.data()), png_buffer.size());
}

std::string generate_qr_png(const std::string &text, const std::string &filename)
{
    QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::LOW);

    const int scale = 10;
    const int border = 4;
    const int size = (qr.getSize() + border * 2) * scale;
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P5\n" << size << " " << size << "\n255\n";

    for (int y = -border; y < qr.getSize() + border; y++)
    {
        for (int sy = 0; sy < scale; sy++)
        {
            for (int x = -border; x < qr.getSize() + border; x++)
            {
                for (int sx = 0; sx < scale; sx++)
                {
                    bool pixel = qr.getModule(x, y);
                    unsigned char color = pixel ? 0 : 255;
                    ofs.write((char *)&color, 1);
                }
            }
        }
    }

    ofs.close();
    return filename;
}

int main()
{
    httplib::Server svr;
    // httplib::SSLServer svr;

    std::string url;
    int Port = 8080;
    std::cout << "Input Your Link: ";
    std::getline(std::cin, url);
    std::cout << "Input Your Port: ";
    std::cin >> Port;

    if (url.empty())
    {
        std::cerr << "URL is empty. Exiting.\n";
        return 1;
    }

    svr.Get("/gen", [&](const httplib::Request &req, httplib::Response &res) 
    {
        std::string png_data;
        try
        {
            png_data = generate_qr_png_memory(url);
            res.set_content(png_data, "image/png");
        }
        catch (const std::exception &ex)
        {
            res.status = 500;
            res.set_content("Internal Error: " + std::string(ex.what()), "text/plain");
        }
        catch (...)
        {
            res.status = 500;
            res.set_content("Unknown error", "text/plain");
        }
    });
    std::cout << "QR-API at http://127.0.0.1:" << Port << "/gen\n";

    if (!svr.is_valid())
    {
        std::cerr << "Server is not valid.\n";
    }
    if (!svr.listen("127.0.0.1", Port))
    {
        std::cerr << "Failed to bind port " << Port << ". Is it already in use?\n";
        return 2;
    }

    return 0;
}