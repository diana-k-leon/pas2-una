// ============================================================
// ejemplo-vector-audio.cpp
// Programación Aplicada al Sonido II — UNA
// Clase 1: Una señal de audio como vector de floats
// ============================================================
//
// En teoría vimos que el audio digital es una secuencia de
// números — muestras tomadas a intervalos regulares.
// Acá lo mismo, en código C++ moderno.
//
// Compilar:  clang++ ejemplo-vector-audio.cpp -o ejemplo
// Ejecutar:  ./ejemplo
// ============================================================

#include <iostream>   // para imprimir en consola
#include <vector>     // para std::vector
#include <cmath>      // para sqrt()

int main() {

    // ----------------------------------------------------------
    // 1. UNA SEÑAL DE AUDIO
    // ----------------------------------------------------------
    // Un vector de floats donde cada número es una muestra.
    // Estos valores representan una onda senoidal simple
    // (una vuelta completa), con amplitud entre -1.0 y 1.0.
    // En un archivo de audio real habría 44100 de estos por segundo.

    std::vector<float> signal = {
        0.0f,  0.5f,  1.0f,  0.5f,
        0.0f, -0.5f, -1.0f, -0.5f
    };

    std::cout << "=== Señal original ===" << std::endl;
    for (int i = 0; i < signal.size(); i++) {
        std::cout << "sample[" << i << "] = " << signal[i] << std::endl;
    }


    // ----------------------------------------------------------
    // 2. ESCALAR EL VOLUMEN (operador LTI lineal más simple)
    // ----------------------------------------------------------
    // Multiplicar cada muestra por un número = subir o bajar volumen.
    // gain > 1.0 → más fuerte
    // gain < 1.0 → más suave
    // Esto es un operador lineal: si doblás el gain, se dobla la salida.

    float gain = 0.5f;  // bajamos el volumen a la mitad

    for (int i = 0; i < signal.size(); i++) {
        signal[i] *= gain;
    }

    std::cout << "\n=== Señal después de bajar volumen (gain = 0.5) ===" << std::endl;
    for (int i = 0; i < signal.size(); i++) {
        std::cout << "sample[" << i << "] = " << signal[i] << std::endl;
    }


    // ----------------------------------------------------------
    // 3. MEZCLAR DOS SEÑALES
    // ----------------------------------------------------------
    // Sumar dos vectores elemento a elemento = mezclar dos pistas.
    // Exactamente lo que hace un mixer: suma las señales.

    std::vector<float> signal2 = {
        0.2f,  0.2f,  0.2f,  0.2f,
        0.2f,  0.2f,  0.2f,  0.2f
    };

    std::vector<float> mix(signal.size());  // vector para el resultado

    for (int i = 0; i < signal.size(); i++) {
        mix[i] = signal[i] + signal2[i];
    }

    std::cout << "\n=== Mezcla de dos señales ===" << std::endl;
    for (int i = 0; i < mix.size(); i++) {
        std::cout << "mix[" << i << "] = " << mix[i] << std::endl;
    }


    // ----------------------------------------------------------
    // 4. CALCULAR RMS (Root Mean Square)
    // ----------------------------------------------------------
    // El RMS es una medida del volumen percibido de una señal.
    // Fórmula: raíz cuadrada del promedio de los cuadrados.
    //
    // ¿Por qué no simplemente el promedio?
    // Porque las muestras negativas cancelarían a las positivas
    // y el promedio de una onda simétrica sería 0 — incorrecto.
    // Al elevar al cuadrado, todos los valores se vuelven positivos.

    float suma = 0.0f;
    for (int i = 0; i < signal.size(); i++) {
        suma += signal[i] * signal[i];  // elevar al cuadrado y acumular
    }
    float rms = sqrt(suma / signal.size());  // raíz del promedio

    std::cout << "\n=== RMS de la señal ===" << std::endl;
    std::cout << "RMS = " << rms << std::endl;
    std::cout << "(cuanto más cerca de 1.0, más fuerte suena)" << std::endl;

    return 0;
}
