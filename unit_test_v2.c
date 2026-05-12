/*
 * Unit Test Bootstrap
 * Autor: Tomasz Jaworski, 2018-2020
 *
 * Test dla zadania Kolokwium 12/05/2023 - zadanie na 3,0 - C1
 * Autor testowanej odpowiedzi: Przemysław  Antosiewicz
 * Test wygenerowano automatycznie o 2026-05-12 11:42:51.151721
 *
 * Debug: 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <setjmp.h>
#include <assert.h>

#if !defined(__clang__) && !defined(__GNUC__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na swoim kompilatorze C.
#error System testow jednostkowych jest przeznaczony dla kompilatorów GCC/Clang.
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// Zakomentuj poniższy błąd, jeżeli chcesz przetestować testy na platformie Windows.
#error System testow jednostkowych NIE jest przeznaczony dla testów uruchamianych na platformach Windows.
#endif

#define _RLDEBUG_API_
#include "unit_helper_v2.h"
#include "rdebug.h"

#include "tested_declarations.h"
#include "rdebug.h"

//
// Elementy globalne dla całego testu
//




//
//  Test 1: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 160 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST1(void)
{
    // informacje o teście
    test_start(1, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 160 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(160);
    
    //
    // -----------
    //
    

                        int array[16][1] = {{ 58}, { 103}, { 167}, { 141}, { 202}, { 235}, { 237}, { 240}, { 214}, { 86}, { 158}, { 38}, { 105}, { 70}, { 205}, { 45}};

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("invent.bin", &error);
                        printf("#####END#####");

                        test_error(error == 0, "Funkcja load_image() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 1, "Funkcja load_image() powinna ustawić szerokość macierzy na 1, a ustawiła na %d", img->width);
                            test_error(img->height == 16, "Funkcja load_image() powinna ustawić pojemność tablicy na 16, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                        
                            for (int i = 0; i < 16; ++i)
                                for (int j = 0; j < 1; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }
                       
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 2: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 34 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST2(void)
{
    // informacje o teście
    test_start(2, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 34 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(34);
    
    //
    // -----------
    //
    

                        int array[1][10] = {{ 244, 246, 173, 4, 126, 175, 231, 33, 63, 201}};

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("plain.bin", &error);
                        printf("#####END#####");

                        test_error(error == 0, "Funkcja load_image() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 10, "Funkcja load_image() powinna ustawić szerokość macierzy na 10, a ustawiła na %d", img->width);
                            test_error(img->height == 1, "Funkcja load_image() powinna ustawić pojemność tablicy na 1, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                        
                            for (int i = 0; i < 1; ++i)
                                for (int j = 0; j < 10; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }
                       
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 3: Sprawdzanie reakcji funkcji load_image
//
void UTEST3(void)
{
    // informacje o teście
    test_start(3, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("leg.bin", &error);
                        printf("#####END#####");

                        test_error(error == 3, "Funkcja load_image() powinna zwrócić kod błędu 3, a zwróciła %d", error);
                        if (!3)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 4: Sprawdzanie reakcji funkcji load_image
//
void UTEST4(void)
{
    // informacje o teście
    test_start(4, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("neighbor.bin", &error);
                        printf("#####END#####");

                        test_error(error == 3, "Funkcja load_image() powinna zwrócić kod błędu 3, a zwróciła %d", error);
                        if (!3)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 5: Sprawdzanie reakcji funkcji load_image
//
void UTEST5(void)
{
    // informacje o teście
    test_start(5, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("soil.bin", &error);
                        printf("#####END#####");

                        test_error(error == 3, "Funkcja load_image() powinna zwrócić kod błędu 3, a zwróciła %d", error);
                        if (!3)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 6: Sprawdzanie reakcji funkcji load_image
//
void UTEST6(void)
{
    // informacje o teście
    test_start(6, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("property.bin", &error);
                        printf("#####END#####");

                        test_error(error == 3, "Funkcja load_image() powinna zwrócić kod błędu 3, a zwróciła %d", error);
                        if (!3)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 7: Sprawdzanie reakcji funkcji load_image
//
void UTEST7(void)
{
    // informacje o teście
    test_start(7, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("spot.bin", &error);
                        printf("#####END#####");

                        test_error(error == 3, "Funkcja load_image() powinna zwrócić kod błędu 3, a zwróciła %d", error);
                        if (!3)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 8: Sprawdzanie reakcji funkcji load_image
//
void UTEST8(void)
{
    // informacje o teście
    test_start(8, "Sprawdzanie reakcji funkcji load_image", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                        int array[9][6] = {{ 9, 2, 5, 2, 7, 10}, { 9, 0, 7, 4, 10, 4}, { 6, 2, 10, 10, 7, 1}, { 10, 3, 0, 8, 2, 4}, { 10, 8, 4, 2, 3, 1}, { 6, 1, 0, 2, 2, 10}, { 10, 4, 4, 1, 8, 4}, { 7, 9, 4, 4, 7, 2}, { 9, 7, 1, 9, 0, 0}};

                        enum error_code_t error;

                        printf("#####START#####");                            
                        struct img_t* img = load_image("page.bin", &error);
                        printf("#####END#####");

                        test_error(error == 2, "Funkcja load_image() powinna zwrócić kod błędu 2, a zwróciła %d", error);
                        if (!2)
                        {
                            test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 6, "Funkcja load_image() powinna ustawić szerokość macierzy na 6, a ustawiła na %d", img->width);
                            test_error(img->height == 9, "Funkcja load_image() powinna ustawić pojemność tablicy na 9, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 9; ++i)
                                for (int j = 0; j < 6; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 9: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 313 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST9(void)
{
    // informacje o teście
    test_start(9, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 313 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(313);
    
    //
    // -----------
    //
    

                int array[11][19] = {{ 231, 233, 0, 102, 68, 53, 83, 110, 126, 77, 220, 178, 67, 210, 101, 235, 2, 118, 181}, { 127, 179, 51, 166, 171, 149, 39, 186, 104, 25, 163, 144, 191, 137, 49, 224, 160, 212, 127}, { 220, 140, 184, 186, 130, 253, 61, 148, 127, 109, 191, 7, 199, 50, 194, 250, 223, 225, 207}, { 6, 208, 133, 252, 162, 209, 1, 81, 39, 61, 21, 92, 235, 222, 251, 135, 9, 234, 207}, { 187, 102, 146, 55, 184, 252, 187, 40, 45, 143, 140, 29, 147, 107, 33, 105, 86, 222, 137}, { 137, 104, 204, 181, 174, 227, 197, 145, 211, 213, 145, 51, 205, 32, 140, 126, 192, 28, 73}, { 20, 175, 141, 53, 244, 250, 205, 57, 72, 136, 146, 51, 254, 61, 122, 29, 168, 88, 89}, { 38, 165, 17, 240, 95, 238, 133, 177, 180, 243, 225, 162, 60, 148, 158, 23, 214, 55, 62}, { 198, 53, 13, 70, 80, 215, 167, 193, 202, 79, 172, 43, 121, 158, 123, 212, 135, 225, 118}, { 53, 1, 75, 201, 167, 81, 113, 247, 27, 47, 101, 104, 167, 112, 223, 17, 31, 153, 21}, { 73, 101, 190, 171, 131, 65, 215, 37, 241, 244, 97, 92, 123, 14, 129, 62, 219, 171, 0}};

                enum error_code_t error;
                        
                printf("#####START#####");                            
                struct img_t* img = load_image("hill.bin", &error);
                printf("#####END#####");

                test_error(error == 0, "Funkcja load_image() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        
                test_error(img != NULL, "Funkcja load_image() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                test_error(img->width == 19, "Funkcja load_image() powinna ustawić szerokość macierzy na 19, a ustawiła na %d", img->width);
                test_error(img->height == 11, "Funkcja load_image() powinna ustawić pojemność tablicy na 11, a ustawiła na %d", img->height);

                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                        
                for (int i = 0; i < 11; ++i)
                    for (int j = 0; j < 19; ++j)
                        test_error(img->img[i][j] == array[i][j], "Funkcja load_image() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                destroy_img(img);                   
                     
                test_no_heap_leakage();
                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 10: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 0 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST10(void)
{
    // informacje o teście
    test_start(10, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 0 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(0);
    
    //
    // -----------
    //
    

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("hill.bin", &error);
                        printf("#####END#####");
        
                        test_error(error == 5, "Funkcja load_image() powinna zwrócić kod błędu 5, a zwróciła %d", error);
                         
                        test_error(img == NULL, "Funkcja load_image() powinna zwrócić NULL");
                        
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 11: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 16 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST11(void)
{
    // informacje o teście
    test_start(11, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 16 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(16);
    
    //
    // -----------
    //
    

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("hill.bin", &error);
                        printf("#####END#####");
        
                        test_error(error == 5, "Funkcja load_image() powinna zwrócić kod błędu 5, a zwróciła %d", error);
                         
                        test_error(img == NULL, "Funkcja load_image() powinna zwrócić NULL");
                        
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 12: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 104 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST12(void)
{
    // informacje o teście
    test_start(12, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 104 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(104);
    
    //
    // -----------
    //
    

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("hill.bin", &error);
                        printf("#####END#####");
        
                        test_error(error == 5, "Funkcja load_image() powinna zwrócić kod błędu 5, a zwróciła %d", error);
                         
                        test_error(img == NULL, "Funkcja load_image() powinna zwrócić NULL");
                        
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 13: Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 123 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST13(void)
{
    // informacje o teście
    test_start(13, "Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 123 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(123);
    
    //
    // -----------
    //
    

                        enum error_code_t error;
                        
                        printf("#####START#####");                            
                        struct img_t* img = load_image("hill.bin", &error);
                        printf("#####END#####");
        
                        test_error(error == 5, "Funkcja load_image() powinna zwrócić kod błędu 5, a zwróciła %d", error);
                         
                        test_error(img == NULL, "Funkcja load_image() powinna zwrócić NULL");
                        
                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 14: Sprawdzanie reakcji funkcji load_image na niepoprawne dane wejściowe
//
void UTEST14(void)
{
    // informacje o teście
    test_start(14, "Sprawdzanie reakcji funkcji load_image na niepoprawne dane wejściowe", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                    enum error_code_t error;
                        
                    printf("#####START#####");                            
                    struct img_t* img = load_image("hill.bin", NULL);
                    printf("#####END#####");
                
                    test_error(img == NULL, "Funkcja load_image() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    img = load_image(NULL, NULL);                
                    test_error(img == NULL, "Funkcja load_image() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    img = load_image(NULL, &error);
                    test_error(error == 1, "Funkcja load_image() powinna zwrócić kod błędu 1, a zwróciła %d", error);
                    test_error(img == NULL, "Funkcja load_image() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    test_no_heap_leakage();
                    onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 15: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 320 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST15(void)
{
    // informacje o teście
    test_start(15, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 320 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(320);
    
    //
    // -----------
    //
    

                        int array[16][1] = {{ 0}, { 0}, { 255}, { 0}, { 255}, { 255}, { 255}, { 255}, { 255}, { 0}, { 255}, { 0}, { 0}, { 0}, { 255}, { 0}};

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("invent.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);
                        
                        test_error(error == 0, "Funkcja image_threshold() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(img != NULL, "Funkcja image_threshold() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 1, "Funkcja image_threshold() powinna ustawić szerokość macierzy na 1, a ustawiła na %d", img->width);
                            test_error(img->height == 16, "Funkcja image_threshold() powinna ustawić pojemność tablicy na 16, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 16; ++i)
                                for (int j = 0; j < 1; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja image_threshold() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);
                            destroy_img(img1);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 16: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 68 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST16(void)
{
    // informacje o teście
    test_start(16, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 68 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(68);
    
    //
    // -----------
    //
    

                        int array[1][10] = {{ 255, 255, 255, 0, 0, 255, 255, 0, 0, 255}};

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("plain.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);
                        
                        test_error(error == 0, "Funkcja image_threshold() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(img != NULL, "Funkcja image_threshold() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(img->width == 10, "Funkcja image_threshold() powinna ustawić szerokość macierzy na 10, a ustawiła na %d", img->width);
                            test_error(img->height == 1, "Funkcja image_threshold() powinna ustawić pojemność tablicy na 1, a ustawiła na %d", img->height);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 1; ++i)
                                for (int j = 0; j < 10; ++j)
                                    test_error(img->img[i][j] == array[i][j], "Funkcja image_threshold() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                            destroy_img(img);
                            destroy_img(img1);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 17: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 662 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST17(void)
{
    // informacje o teście
    test_start(17, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 662 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(662);
    
    //
    // -----------
    //
    

                int array[15][13] = {{ 0, 255, 255, 0, 255, 0, 255, 255, 255, 255, 0, 0, 0}, { 255, 0, 255, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255}, { 255, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255, 0, 255}, { 255, 255, 0, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255}, { 0, 255, 0, 0, 255, 255, 0, 255, 0, 0, 255, 0, 255}, { 0, 0, 0, 255, 255, 0, 255, 0, 255, 0, 255, 255, 255}, { 0, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0}, { 0, 0, 0, 0, 0, 255, 255, 255, 0, 255, 0, 255, 0}, { 255, 0, 255, 255, 255, 0, 0, 255, 0, 0, 255, 255, 0}, { 255, 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 255, 255}, { 255, 0, 0, 0, 0, 0, 255, 0, 255, 0, 255, 255, 255}, { 255, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 255, 0}, { 0, 255, 255, 0, 255, 255, 0, 255, 255, 0, 0, 0, 0}, { 0, 255, 0, 255, 255, 0, 255, 0, 0, 255, 0, 255, 0}, { 0, 255, 0, 255, 255, 0, 0, 255, 255, 0, 0, 255, 255}};

                enum error_code_t error1, error;

                printf("#####START#####");                            
                struct img_t* img1 = load_image("seed.bin", &error1);
                printf("#####END#####");
                        
                struct img_t* img = image_threshold(img1, &error);

                test_error(error == 0, "Funkcja image_threshold() powinna zwrócić kod błędu 0, a zwróciła %d", error);

                test_error(img != NULL, "Funkcja image_threshold() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                test_error(img->width == 13, "Funkcja image_threshold() powinna ustawić szerokość macierzy na 13, a ustawiła na %d", img->width);
                test_error(img->height == 15, "Funkcja image_threshold() powinna ustawić pojemność tablicy na 15, a ustawiła na %d", img->height);

                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                for (int i = 0; i < 15; ++i)
                    for (int j = 0; j < 13; ++j)
                        test_error(img->img[i][j] == array[i][j], "Funkcja image_threshold() niepoprawnie wczytała dane w komórce (%d, %d) powinno być %d, a jest %d", i, j, array[i][j], img->img[i][j]);

                destroy_img(img);
                destroy_img(img1);                   

                test_no_heap_leakage();
                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 18: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 331 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST18(void)
{
    // informacje o teście
    test_start(18, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 331 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(331);
    
    //
    // -----------
    //
    

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("seed.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);

                        test_error(error == 5, "Funkcja image_threshold() powinna zwrócić kod błędu 5, a zwróciła %d", error);

                        test_error(img == NULL, "Funkcja image_threshold() powinna zwrócić NULL");

                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 19: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 347 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST19(void)
{
    // informacje o teście
    test_start(19, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 347 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(347);
    
    //
    // -----------
    //
    

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("seed.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);

                        test_error(error == 5, "Funkcja image_threshold() powinna zwrócić kod błędu 5, a zwróciła %d", error);

                        test_error(img == NULL, "Funkcja image_threshold() powinna zwrócić NULL");

                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 20: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 467 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST20(void)
{
    // informacje o teście
    test_start(20, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 467 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(467);
    
    //
    // -----------
    //
    

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("seed.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);

                        test_error(error == 5, "Funkcja image_threshold() powinna zwrócić kod błędu 5, a zwróciła %d", error);

                        test_error(img == NULL, "Funkcja image_threshold() powinna zwrócić NULL");

                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 21: Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 519 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST21(void)
{
    // informacje o teście
    test_start(21, "Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 519 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(519);
    
    //
    // -----------
    //
    

                        enum error_code_t error1, error;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("seed.bin", &error1);
                        printf("#####END#####");
                        
                        struct img_t* img = image_threshold(img1, &error);

                        test_error(error == 5, "Funkcja image_threshold() powinna zwrócić kod błędu 5, a zwróciła %d", error);

                        test_error(img == NULL, "Funkcja image_threshold() powinna zwrócić NULL");

                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 22: Sprawdzanie reakcji funkcji image_threshold na niepoprawne dane wejściowe
//
void UTEST22(void)
{
    // informacje o teście
    test_start(22, "Sprawdzanie reakcji funkcji image_threshold na niepoprawne dane wejściowe", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                    enum error_code_t error1, error;

                    printf("#####START#####");                            
                    struct img_t* img1 = load_image("seed.bin", &error1);
                    printf("#####END#####");
                        
                    struct img_t* img = image_threshold(img1, NULL);

                    test_error(img == NULL, "Funkcja image_threshold() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    img = image_threshold(NULL, NULL);                
                    test_error(img == NULL, "Funkcja image_threshold() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    img = image_threshold(NULL, &error);
                    test_error(error == 1, "Funkcja image_threshold() powinna zwrócić kod błędu 1, a zwróciła %d", error);
                    test_error(img == NULL, "Funkcja image_threshold() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    destroy_img(img1);

                    test_no_heap_leakage();
                    onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 23: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 512 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST23(void)
{
    // informacje o teście
    test_start(23, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 512 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(512);
    
    //
    // -----------
    //
    

                        int array[16][3] = {{ 0, 0, 1}, { 1, 0, 1}, { 2, 0, 1}, { 3, 0, 1}, { 4, 0, 1}, { 5, 0, 1}, { 6, 0, 1}, { 7, 0, 1}, { 8, 0, 1}, { 9, 0, 1}, { 10, 0, 1}, { 11, 0, 1}, { 12, 0, 1}, { 13, 0, 1}, { 14, 0, 1}, { 15, 0, 1}};

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("invent.bin", &error1);
                        printf("#####END#####");

                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == 0, "Funkcja area_statistics() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(areas != NULL, "Funkcja area_statistics() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(counter == 16, "Funkcja area_statistics() powinna ustawić licznik na 16, a ustawiła na %d", counter);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 16; ++i)
                            {
                                test_error(areas[i].size == array[i][2], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][2], areas[i].size);
                                test_error(areas[i].left_x == array[i][1], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][1], areas[i].left_x);
                                test_error(areas[i].top_y == array[i][0], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][0], areas[i].top_y);
                            }
                            
                            free(areas);
                            destroy_img(img1);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 24: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 188 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST24(void)
{
    // informacje o teście
    test_start(24, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 188 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(188);
    
    //
    // -----------
    //
    

                        int array[10][3] = {{ 0, 0, 1}, { 0, 1, 1}, { 0, 2, 1}, { 0, 3, 1}, { 0, 4, 1}, { 0, 5, 1}, { 0, 6, 1}, { 0, 7, 1}, { 0, 8, 1}, { 0, 9, 1}};

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;

                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("plain.bin", &error1);
                        printf("#####END#####");

                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == 0, "Funkcja area_statistics() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        if (!0)
                        {
                            test_error(areas != NULL, "Funkcja area_statistics() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                            test_error(counter == 10, "Funkcja area_statistics() powinna ustawić licznik na 10, a ustawiła na %d", counter);

                            onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                            for (int i = 0; i < 10; ++i)
                            {
                                test_error(areas[i].size == array[i][2], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][2], areas[i].size);
                                test_error(areas[i].left_x == array[i][1], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][1], areas[i].left_x);
                                test_error(areas[i].top_y == array[i][0], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][0], areas[i].top_y);
                            }
                            
                            free(areas);
                            destroy_img(img1);                     
                        }

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 25: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 4544 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
//
void UTEST25(void)
{
    // informacje o teście
    test_start(25, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 4544 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(4544);
    
    //
    // -----------
    //
    

                int array[289][3] = {{ 0, 0, 1}, { 0, 1, 1}, { 0, 2, 1}, { 0, 3, 1}, { 0, 4, 1}, { 0, 5, 1}, { 0, 6, 1}, { 0, 7, 1}, { 0, 8, 1}, { 0, 9, 1}, { 1, 0, 1}, { 1, 1, 1}, { 1, 2, 1}, { 1, 3, 1}, { 1, 4, 1}, { 1, 5, 1}, { 1, 6, 1}, { 1, 7, 1}, { 1, 8, 1}, { 1, 9, 1}, { 2, 0, 1}, { 2, 1, 1}, { 2, 2, 1}, { 2, 3, 1}, { 2, 4, 1}, { 2, 5, 1}, { 2, 6, 1}, { 2, 7, 1}, { 2, 8, 1}, { 2, 9, 1}, { 3, 0, 1}, { 3, 1, 1}, { 3, 2, 1}, { 3, 3, 1}, { 3, 4, 1}, { 3, 5, 1}, { 3, 6, 1}, { 3, 7, 1}, { 3, 8, 1}, { 3, 9, 1}, { 4, 0, 1}, { 4, 1, 1}, { 4, 2, 1}, { 4, 3, 1}, { 4, 4, 1}, { 4, 5, 1}, { 4, 6, 1}, { 4, 7, 1}, { 4, 8, 1}, { 4, 9, 1}, { 5, 0, 1}, { 5, 1, 1}, { 5, 2, 1}, { 5, 3, 1}, { 5, 4, 1}, { 5, 5, 1}, { 5, 6, 1}, { 5, 7, 1}, { 5, 8, 1}, { 5, 9, 1}, { 6, 0, 1}, { 6, 1, 1}, { 6, 2, 1}, { 6, 3, 1}, { 6, 4, 1}, { 6, 5, 1}, { 6, 6, 1}, { 6, 7, 1}, { 6, 8, 1}, { 6, 9, 1}, { 7, 0, 1}, { 7, 1, 1}, { 7, 2, 1}, { 7, 3, 1}, { 7, 4, 1}, { 7, 5, 1}, { 7, 6, 1}, { 7, 7, 1}, { 7, 8, 1}, { 7, 9, 1}, { 8, 0, 1}, { 8, 1, 1}, { 8, 2, 1}, { 8, 3, 1}, { 8, 4, 1}, { 8, 5, 1}, { 8, 6, 1}, { 8, 7, 1}, { 8, 8, 1}, { 8, 9, 1}, { 9, 0, 1}, { 9, 1, 1}, { 9, 2, 1}, { 9, 3, 1}, { 9, 4, 1}, { 9, 5, 1}, { 9, 6, 1}, { 9, 7, 1}, { 9, 8, 1}, { 9, 9, 1}, { 10, 0, 1}, { 10, 1, 1}, { 10, 2, 1}, { 10, 3, 1}, { 10, 4, 1}, { 10, 5, 1}, { 10, 6, 1}, { 10, 7, 1}, { 10, 8, 1}, { 10, 9, 1}, { 11, 0, 1}, { 11, 1, 1}, { 11, 2, 1}, { 11, 3, 1}, { 11, 4, 1}, { 11, 5, 1}, { 11, 6, 1}, { 11, 7, 1}, { 11, 8, 1}, { 11, 9, 1}, { 12, 0, 1}, { 12, 1, 1}, { 12, 2, 1}, { 12, 3, 1}, { 12, 4, 1}, { 12, 5, 1}, { 12, 6, 1}, { 12, 7, 1}, { 12, 8, 1}, { 12, 9, 1}, { 13, 0, 1}, { 13, 1, 1}, { 13, 2, 1}, { 13, 3, 1}, { 13, 4, 1}, { 13, 5, 1}, { 13, 6, 1}, { 13, 7, 1}, { 13, 8, 1}, { 13, 9, 1}, { 14, 0, 1}, { 14, 1, 1}, { 14, 2, 1}, { 14, 3, 1}, { 14, 4, 1}, { 14, 5, 1}, { 14, 6, 1}, { 14, 7, 1}, { 14, 8, 1}, { 14, 9, 1}, { 15, 0, 1}, { 15, 1, 1}, { 15, 2, 1}, { 15, 3, 1}, { 15, 4, 1}, { 15, 5, 1}, { 15, 6, 1}, { 15, 7, 1}, { 15, 8, 1}, { 15, 9, 1}, { 16, 0, 1}, { 16, 1, 1}, { 16, 2, 1}, { 16, 3, 1}, { 16, 4, 1}, { 16, 5, 1}, { 16, 6, 1}, { 16, 7, 1}, { 16, 8, 1}, { 16, 9, 1}, { 17, 0, 1}, { 17, 1, 1}, { 17, 2, 1}, { 17, 3, 1}, { 17, 4, 2}, { 17, 5, 1}, { 17, 6, 1}, { 17, 7, 1}, { 17, 8, 1}, { 17, 9, 1}, { 18, 0, 1}, { 18, 1, 1}, { 18, 2, 1}, { 18, 3, 1}, { 18, 4, 1}, { 18, 6, 1}, { 18, 7, 1}, { 18, 8, 1}, { 18, 9, 1}, { 19, 0, 1}, { 19, 1, 1}, { 19, 2, 1}, { 19, 3, 1}, { 19, 4, 1}, { 19, 5, 1}, { 19, 6, 1}, { 19, 7, 1}, { 19, 8, 1}, { 19, 9, 1}, { 20, 0, 1}, { 20, 1, 1}, { 20, 2, 1}, { 20, 3, 1}, { 20, 4, 1}, { 20, 5, 1}, { 20, 6, 1}, { 20, 7, 1}, { 20, 8, 1}, { 20, 9, 1}, { 21, 0, 1}, { 21, 1, 1}, { 21, 2, 1}, { 21, 3, 1}, { 21, 4, 1}, { 21, 5, 1}, { 21, 6, 1}, { 21, 7, 1}, { 21, 8, 1}, { 21, 9, 1}, { 22, 0, 1}, { 22, 1, 1}, { 22, 2, 1}, { 22, 3, 1}, { 22, 4, 1}, { 22, 5, 1}, { 22, 6, 1}, { 22, 7, 1}, { 22, 8, 1}, { 22, 9, 1}, { 23, 0, 1}, { 23, 1, 1}, { 23, 2, 1}, { 23, 3, 1}, { 23, 4, 1}, { 23, 5, 1}, { 23, 6, 1}, { 23, 7, 1}, { 23, 8, 1}, { 23, 9, 1}, { 24, 0, 1}, { 24, 1, 1}, { 24, 2, 1}, { 24, 3, 1}, { 24, 4, 1}, { 24, 5, 1}, { 24, 6, 1}, { 24, 7, 1}, { 24, 8, 1}, { 24, 9, 1}, { 25, 0, 1}, { 25, 1, 1}, { 25, 2, 1}, { 25, 3, 1}, { 25, 4, 1}, { 25, 5, 1}, { 25, 6, 1}, { 25, 7, 1}, { 25, 8, 1}, { 25, 9, 1}, { 26, 0, 1}, { 26, 1, 1}, { 26, 2, 1}, { 26, 3, 1}, { 26, 4, 1}, { 26, 5, 1}, { 26, 6, 1}, { 26, 7, 1}, { 26, 8, 1}, { 26, 9, 1}, { 27, 0, 1}, { 27, 1, 1}, { 27, 2, 1}, { 27, 3, 1}, { 27, 4, 1}, { 27, 5, 1}, { 27, 6, 1}, { 27, 7, 1}, { 27, 8, 1}, { 27, 9, 1}, { 28, 0, 1}, { 28, 1, 1}, { 28, 2, 1}, { 28, 3, 1}, { 28, 4, 1}, { 28, 5, 1}, { 28, 6, 1}, { 28, 7, 1}, { 28, 8, 1}, { 28, 9, 1}};

                enum error_code_t error1;
                struct area_t* areas = NULL;
                int counter = 0;

                printf("#####START#####");                            
                struct img_t* img1 = load_image("foot.bin", &error1);
                printf("#####END#####");

                int error = area_statistics(img1, &areas, &counter);

                test_error(error == 0, "Funkcja area_statistics() powinna zwrócić kod błędu 0, a zwróciła %d", error);
                        
                test_error(areas != NULL, "Funkcja area_statistics() powinna zwrócić adres zaalokowanej pamięci, a zwróciła NULL");
                test_error(counter == 289, "Funkcja area_statistics() powinna ustawić licznik na 289, a ustawiła na %d", counter);

                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)

                for (int i = 0; i < 289; ++i)
                {
                    test_error(areas[i].size == array[i][2], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][2], areas[i].size);
                    test_error(areas[i].left_x == array[i][1], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][1], areas[i].left_x);
                    test_error(areas[i].top_y == array[i][0], "Funkcja area_statistics() niepoprawnie wczytała dane w komórce (%d) powinno być %d, a jest %d", i, array[i][0], areas[i].top_y);
                }
                
                free(areas);
                destroy_img(img1);  

                test_no_heap_leakage();
                onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
            
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 26: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 538 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST26(void)
{
    // informacje o teście
    test_start(26, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 538 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(538);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 27: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 554 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST27(void)
{
    // informacje o teście
    test_start(27, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 554 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(554);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 28: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 786 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST28(void)
{
    // informacje o teście
    test_start(28, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 786 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(786);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 29: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 806 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST29(void)
{
    // informacje o teście
    test_start(29, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 806 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(806);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 30: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1060 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST30(void)
{
    // informacje o teście
    test_start(30, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1060 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(1060);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 31: Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1180 bajtów) - niewystarczająco pamięci na wczytanie danych
//
void UTEST31(void)
{
    // informacje o teście
    test_start(31, "Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1180 bajtów) - niewystarczająco pamięci na wczytanie danych", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    rldebug_heap_set_global_limit(1180);
    
    //
    // -----------
    //
    

                        enum error_code_t error1;
                        struct area_t* areas = NULL;
                        int counter = 0;
            
                        printf("#####START#####");                            
                        struct img_t* img1 = load_image("foot.bin", &error1);
                        printf("#####END#####");
            
                        int error = area_statistics(img1, &areas, &counter);

                        test_error(error == -2, "Funkcja area_statistics() powinna zwrócić kod błędu -2, a zwróciła %d", error);

                        test_error(areas == NULL, "Funkcja area_statistics() powinna zwrócić NULL");

                        free(areas);
                        destroy_img(img1);

                        test_no_heap_leakage();
                        onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                    
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}

//
//  Test 32: Sprawdzanie reakcji funkcji area_statistics na niepoprawne dane wejściowe
//
void UTEST32(void)
{
    // informacje o teście
    test_start(32, "Sprawdzanie reakcji funkcji area_statistics na niepoprawne dane wejściowe", __LINE__);

    // uwarunkowanie zasobów - pamięci, itd...
    test_file_write_limit_setup(33554432);
    rldebug_reset_limits();
    
    //
    // -----------
    //
    

                    enum error_code_t error1;
                    struct area_t* areas = NULL;
                    int counter = 0;
        
                    printf("#####START#####");                            
                    struct img_t* img1 = load_image("foot.bin", &error1);
                    printf("#####END#####");
        
                    int error = area_statistics(img1, &areas, NULL);

                    test_error(areas == NULL, "Funkcja area_statistics() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    error = area_statistics(img1, NULL, NULL);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);

                    error = area_statistics(NULL, NULL, NULL);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);

                    error = area_statistics(NULL, &areas, NULL);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);
                    test_error(areas == NULL, "Funkcja area_statistics() powinna przypisać NULL pod wskaźnik przekazany w parametrze");

                    error = area_statistics(img1, NULL, &counter);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);
                    test_error(counter == 0, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);

                    error = area_statistics(NULL, &areas, &counter);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);
                    test_error(areas == NULL, "Funkcja area_statistics() powinna przypisać NULL pod wskaźnik przekazany w parametrze");
                    test_error(counter == 0, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);
                    
                    error = area_statistics(NULL, NULL, &counter);                
                    test_error(error == -1, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);
                    test_error(counter == 0, "Funkcja area_statistics() powinna zwrócić kod błędu -1, a zwróciła %d", error);

                    free(areas);
                    destroy_img(img1);

                    test_no_heap_leakage();
                    onerror_terminate(); // przerwanie wszystkich testów jednostkowych (np. coś jest mocno nie tak z kodem)
                
    //
    // -----------
    //

    // przywrócenie podstawowych parametów przydzielania zasobów (jeśli to tylko możliwe)
    rldebug_reset_limits();
    test_file_write_limit_restore();
    
    test_ok();
}




enum run_mode_t { rm_normal_with_rld = 0, rm_unit_test = 1, rm_main_test = 2 };

int __wrap_main(volatile int _argc, char** _argv, char** _envp)
{
    int volatile vargc = _argc;
    char ** volatile vargv = _argv, ** volatile venvp = _envp;
	volatile enum run_mode_t run_mode = rm_unit_test; // -1
	volatile int selected_test = -1;

    if (vargc > 1)
	{
	    char* smode = strtok(vargv[1], ",");
	    char* stest = strtok(NULL, "");
		char *errptr = NULL;
		run_mode = (enum run_mode_t)strtol(smode, &errptr, 10);
		if (*errptr == '\x0')
		{
			memmove(vargv + 1, vargv + 2, sizeof(char*) * (vargc - 1));
			vargc--;

			if (stest != NULL)
			{
			    int val = (int)strtol(stest, &errptr, 10);
			    if (*errptr == '\x0')
			        selected_test = val;
			}
		}
	}

    // printf("runmode=%d; selected_test=%d\n", run_mode, selected_test);

    // inicjuj testy jednostkowe
    unit_test_init(run_mode, "unit_test_v2.c");
    test_limit_init();
    rldebug_set_reported_severity_level(MSL_FAILURE);

    if (run_mode == rm_normal_with_rld)
    {
        // konfiguracja ograniczników
        rldebug_reset_limits();
        

        // uruchom funkcję main Studenta a potem wyświetl podsumowanie sterty i zasobów
        volatile int ret_code = rdebug_call_main(tested_main, vargc, vargv, venvp);

        rldebug_reset_limits();
        

        int leaks_detected = rldebug_show_leaked_resources(0);
        if (leaks_detected)
            raise(SIGHEAP);

        return ret_code;
    }

    
    if (run_mode == rm_unit_test)
    {
        test_title("Testy jednostkowe");

        void (*pfcn[])(void) =
        { 
            UTEST1, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 160 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST2, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 34 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST3, // Sprawdzanie reakcji funkcji load_image
            UTEST4, // Sprawdzanie reakcji funkcji load_image
            UTEST5, // Sprawdzanie reakcji funkcji load_image
            UTEST6, // Sprawdzanie reakcji funkcji load_image
            UTEST7, // Sprawdzanie reakcji funkcji load_image
            UTEST8, // Sprawdzanie reakcji funkcji load_image
            UTEST9, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 313 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST10, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 0 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST11, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 16 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST12, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 104 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST13, // Sprawdzanie reakcji funkcji load_image na limit pamięci (limit sterty ustawiono na 123 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST14, // Sprawdzanie reakcji funkcji load_image na niepoprawne dane wejściowe
            UTEST15, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 320 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST16, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 68 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST17, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 662 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST18, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 331 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST19, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 347 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST20, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 467 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST21, // Sprawdzanie reakcji funkcji image_threshold na limit pamięci (limit sterty ustawiono na 519 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST22, // Sprawdzanie reakcji funkcji image_threshold na niepoprawne dane wejściowe
            UTEST23, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 512 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST24, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 188 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST25, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 4544 bajtów - wystarczająco na wczytanie wszystkich danych z pliku)
            UTEST26, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 538 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST27, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 554 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST28, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 786 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST29, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 806 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST30, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1060 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST31, // Sprawdzanie reakcji funkcji area_statistics na limit pamięci (limit sterty ustawiono na 1180 bajtów) - niewystarczająco pamięci na wczytanie danych
            UTEST32, // Sprawdzanie reakcji funkcji area_statistics na niepoprawne dane wejściowe
            NULL
        };

        for (int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx]();

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(32); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem
        return EXIT_SUCCESS;
    }
    

    if (run_mode == rm_main_test)
    {
        test_title("Testy funkcji main()");

        void (*pfcn[])(int, char**, char**) =
        { 
            NULL
        };

        for (volatile int idx = 0; pfcn[idx] != NULL && !test_get_session_termination_flag(); idx++)
        {
            if (selected_test == -1 || selected_test == idx + 1)
                pfcn[idx](vargc, vargv, venvp);

            // limit niezaliczonych testów, po jakim testy jednostkowe zostaną przerwane
            if (test_session_get_fail_count() >= 1000)
                test_terminate_session();
        }


        test_title("RLDebug :: Analiza wycieku zasobów");
        // sprawdź wycieki pamięci
        int leaks_detected = rldebug_show_leaked_resources(1);
        test_set_session_leaks(leaks_detected);

        // poinformuj serwer Mrówka o wyniku testu - podsumowanie
        test_title("Podsumowanie");
        if (selected_test == -1)
            test_summary(0); // wszystkie testy muszą zakończyć się sukcesem
        else
            test_summary(1); // tylko jeden (selected_test) test musi zakończyć się  sukcesem

        return EXIT_SUCCESS;
    }

    printf("*** Nieznana wartość RunMode: %d", (int)run_mode);
    abort();
}