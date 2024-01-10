#include <iostream>
#include <fstream>
#include <cmath>
#include <optional>
#include <string>
#include <cstdlib>
#include <Windows.h>

unsigned char enc_bmp[54] = { 0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned short ancho_in = 4;
unsigned short alto_in = 4;
constexpr unsigned short ancho_fin = 1920;
constexpr unsigned short alto_fin = 1080;

std::string rgb_entrada;
std::optional<float> rgb_salida[ancho_fin * alto_fin * 3];

void redim_img()
{
    unsigned short ancho_min;
    unsigned short alto_min;
    unsigned int iter1 = 0, iter2 = 0;
    unsigned short norm_x = 0;
    unsigned int norm_y = 0;
    
    if(ancho_in <= ancho_fin)
    {
        ancho_min = ancho_in;
    }
    else
    {
        ancho_min = ancho_fin;
    }
    
    if(alto_in <= alto_fin)
    {
        alto_min = alto_in;
    }
    else
    {
        alto_min = alto_fin;
    }
    
    for (unsigned short filas_y = 0; filas_y < alto_min; filas_y++)
    {
        for (unsigned short columnas_x = 0; columnas_x < ancho_min; columnas_x++)
        {
            if(ancho_fin >= ancho_in)
            {
                norm_x = columnas_x * (float(ancho_fin) / (ancho_in - 1));
            }
            else
            {
                norm_x = columnas_x * (float(ancho_in) / (ancho_fin - 1));
            }
            
            if (norm_x == ancho_fin)
            {
                norm_x = ancho_fin - 1;
            }

            norm_x *= 3;
            
            if(alto_fin >= alto_in)
            {
                norm_y = filas_y * (float(alto_fin) / (alto_in - 1));
            }
            else
            {
                norm_y = filas_y * (float(alto_in) / (alto_fin - 1));
            }

            if (norm_y == alto_fin)
            {
                norm_y = alto_fin - 1;
            }

            norm_y *= ancho_fin * 3;
            
            for (unsigned short cont_rgb = 0; cont_rgb < 3; cont_rgb++)
            {
                if(ancho_fin >= ancho_in && alto_fin >= alto_in)
                {
                    rgb_salida[norm_y + norm_x + cont_rgb] = static_cast<unsigned char>(rgb_entrada[(filas_y * ancho_in * 3) + (columnas_x * 3) + cont_rgb]);
                }
                
                if(ancho_fin <= ancho_in && alto_fin < alto_in || ancho_fin < ancho_in && alto_fin <= alto_in)
                {
                    rgb_salida[(filas_y * ancho_in * 3) + (columnas_x * 3) + cont_rgb] = static_cast<unsigned char>(rgb_entrada[norm_y + norm_x + cont_rgb]);
                }
                
                if(ancho_fin > ancho_in && alto_fin < alto_in)
                {
                    rgb_salida[(filas_y * ancho_fin * 3) + norm_x + cont_rgb] = static_cast<unsigned char>(rgb_entrada[norm_y + (columnas_x * 3) + cont_rgb]);
                }
                
                if(ancho_fin < ancho_in && alto_fin > alto_in)
                {
                    rgb_salida[norm_y + (columnas_x * 3) + cont_rgb] = static_cast<unsigned char>(rgb_entrada[(filas_y * ancho_fin * 3) + norm_x + cont_rgb]);
                }
            }
        }
    }

    for (unsigned short filas_y = 0; filas_y < alto_fin; filas_y++)
    {
        for (unsigned short columnas_x = 0; columnas_x < ancho_fin; columnas_x++)
        {
            if (rgb_salida[filas_y * ancho_fin * 3].has_value())
            {
                if (columnas_x == 0)
                {
                    iter1 = iter2;
                    iter2 += 3;
                }
                else
                {
                    if (rgb_salida[iter1].has_value() && rgb_salida[iter2].has_value())
                    {
                        for (unsigned short cont_rgb = 0; cont_rgb < 3; cont_rgb++)
                        {
                            for (unsigned short iter_esp = 0; iter_esp < (iter2 / 3) - (iter1 / 3) - 1; iter_esp++)
                            {
                                rgb_salida[iter1 + 3 + (iter_esp * 3) + cont_rgb] = rgb_salida[iter1 + cont_rgb].value() + ((1 + iter_esp) * (1.0f / ((iter2 / 3) - (iter1 / 3))) * (rgb_salida[iter2 + cont_rgb].value() - rgb_salida[iter1 + cont_rgb].value()));
                            }
                        }

                        iter1 = iter2;
                        iter2 += 3;
                    }
                    else
                    {
                        iter2 += 3;
                    }
                }
            }
            else
            {
                iter1 += 3;
                iter2 += 3;
            }
        }
    }

    for (unsigned short columnas_x = 0; columnas_x < ancho_fin; columnas_x++)
    {
        iter1 = columnas_x * 3, iter2 = columnas_x * 3;

        for (unsigned short filas_y = 0; filas_y < alto_fin; filas_y++)
        {
            if (rgb_salida[filas_y * ancho_fin * 3].has_value())
            {
                if (filas_y == 0)
                {
                    iter2 += ancho_fin * 3;
                }
                else
                {
                    if (rgb_salida[iter1].has_value() && rgb_salida[iter2].has_value())
                    {
                        for (unsigned short cont_rgb = 0; cont_rgb < 3; cont_rgb++)
                        {
                            for (unsigned short iter_esp = 0; iter_esp < (iter2 / ancho_fin / 3) - (iter1 / ancho_fin / 3) - 1; iter_esp++)
                            {
                                rgb_salida[iter1 + (ancho_fin * 3) + (iter_esp * (ancho_fin * 3)) + cont_rgb] = rgb_salida[iter1 + cont_rgb].value() + ((1 + iter_esp) * (1.0f / ((iter2 / ancho_fin / 3) - (iter1 / ancho_fin / 3))) * (rgb_salida[iter2 + cont_rgb].value() - rgb_salida[iter1 + cont_rgb].value()));
                            }
                        }

                        iter1 = iter2;
                        iter2 += ancho_fin * 3;
                    }
                    else
                    {
                        iter2 += ancho_fin * 3;
                    }
                }
            }
            else
            {
                iter2 += ancho_fin * 3;
            }
        }
    }

    for (unsigned int iter = 0; iter < (ancho_fin * alto_fin * 3); iter++)
    {
        rgb_salida[iter] = std::round(rgb_salida[iter].value());
    }
}

int main()
{
    wchar_t w_usr[256];
    DWORD tam_w_usr = 256;

    GetUserName(w_usr, &tam_w_usr);
    std::string n_usr(w_usr, w_usr + tam_w_usr);
    n_usr.pop_back();
    
    enc_bmp[18] = static_cast<unsigned char>(ancho_fin - (unsigned char(ancho_fin / 256) * 256));
    enc_bmp[19] = static_cast<unsigned char>(ancho_fin / 256);
    enc_bmp[22] = static_cast<unsigned char>(alto_fin - (unsigned char(alto_fin / 256) * 256));
    enc_bmp[23] = static_cast<unsigned char>(alto_fin / 256);

    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0x00);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0xA0);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0x50);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);
    rgb_entrada.push_back(0xF0);

    std::string bmp_salida;

    for (unsigned char ch_ch : enc_bmp)
    {
        bmp_salida.push_back(ch_ch);
    }

    redim_img();

    for (std::optional<unsigned char> ch_ch : rgb_salida)
    {
        bmp_salida.push_back(ch_ch.value());
    }

    std::ofstream bmp_arch("C:/Users/" + n_usr + "/Desktop/bmp_example.bmp", std::ios::binary);
    bmp_arch.write(bmp_salida.data(), bmp_salida.size());
    bmp_arch.close();

    return 0;
}
