#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

// =====================================================
// PASO 1. Tipos compuestos y agregados CREADO POR CARLOS
// =====================================================

// 1.1 Convierte segundos a texto m:ss
std::string formatoTiempo(int segundos) {
    std::ostringstream ss;
    ss << segundos / 60 << ":" << std::setw(2) << std::setfill('0') << segundos % 60;
    return ss.str();
}

// 1.2 Tipo Cancion
struct Cancion {
    std::string titulo;   
    std::string artista; 
    int duracion = 0;    

    // Devuelve "Titulo - Artista (m:ss)"
    std::string formato() const {
        return titulo + " - " + artista + " (" + formatoTiempo(duracion) + ")";
    }
};

// 1.3 Tipo Accion
struct Accion {
    std::string tipo;
    Cancion cancion;   
    int posicion = 0;  
};

// 1.4 Tipo Album
struct Album {
    static const int TOTAL = 4;  
    std::string titulo;          
    int anio = 0;               
    Cancion canciones[TOTAL];    

    // Devuelve cuantas canciones tiene
    int cantidad() const { return TOTAL; }

    // Devuelve la suma de las duraciones en segundos
    int duracionTotal() const {
        int total = 0;
        for (int i = 0; i < TOTAL; i++) total += canciones[i].duracion;
        return total;
    }
};

// Crea el album con las cuatro canciones
Album crearAlbum() {
    Album a;
    a.titulo = "Mis favoritas";
    a.anio = 2026;
    a.canciones[0] = {"Do I Wanna Know?", "Arctic Monkeys", 272};
    a.canciones[1] = {"Feel Good Inc.", "Gorillaz", 221};
    a.canciones[2] = {"California Love", "2Pac", 285};
    a.canciones[3] = {"Volver, Volver", "Vicente Fernández", 184};
    return a;
}

// 1.5 Muestra el album
void paso1() {
    Album album = crearAlbum();
    std::cout << album.titulo << " (" << album.anio << ") · " << album.cantidad() << " canciones" << std::endl;
    for (int i = 0; i < album.cantidad(); i++)
        std::cout << i + 1 << ". " << album.canciones[i].formato() << std::endl;
    std::cout << "Duración del album: " << formatoTiempo(album.duracionTotal()) << std::endl;
}

// =====================================================
// PASO 2. Pila estatica CREADO POR CARLOS
// =====================================================

// 2.1 y 2.2 Plantilla con arreglo nativo de capacidad 3
template <typename T>
class PilaEstatica {
private:
    static const int CAPACIDAD = 3;  
    T elementos[CAPACIDAD];         
    int tope = 0;                  

public:
    // Agrega un elemento al tope
    void push(const T& elemento) {
        if (estaLlena()) throw std::runtime_error("Pila llena: no cabe otro elemento");
        elementos[tope++] = elemento;
    }

    // Saca y devuelve el elemento del tope
    T pop() {
        if (estaVacia()) throw std::runtime_error("Pila vacia: no hay nada que sacar");
        T elemento = elementos[--tope];
        elementos[tope] = T();  // limpia la casilla liberada
        return elemento;
    }

    // Devuelve el elemento del tope sin sacarlo
    T peek() const {
        if (estaVacia()) throw std::runtime_error("Pila vacia: no hay tope que ver");
        return elementos[tope - 1];
    }

    // true si no hay elementos
    bool estaVacia() const { return tope == 0; }

    // true si se alcanzo la capacidad
    bool estaLlena() const { return tope == CAPACIDAD; }

    // Cantidad de elementos
    int tamano() const { return tope; }

    // Tamano maximo
    int capacidad() const { return CAPACIDAD; }
};

// =====================================================
// PASO 3. Pila dinamica - CREADO POR JORGE PAZ
// =====================================================

// 3.1 y 3.2 Plantilla con std::vector
template <typename T>
class PilaDinamica {
private:
    std::vector<T> elementos;  

public:
    // Agrega un elemento al tope
    void push(const T& elemento) { elementos.push_back(elemento); }

    // Saca y devuelve el elemento del tope
    T pop() {
        if (estaVacia()) throw std::runtime_error("Pila vacia: no hay nada que sacar");
        T elemento = elementos.back();
        elementos.pop_back();
        return elemento;
    }

    // Devuelve el elemento del tope sin sacarlo
    T peek() const {
        if (estaVacia()) throw std::runtime_error("Pila vacia: no hay tope que ver");
        return elementos.back();
    }

    // true si no hay elementos
    bool estaVacia() const { return elementos.empty(); }

    // Cantidad de elementos
    int tamano() const { return (int)elementos.size(); }
};

// 2.4 y 3.3 Prueba de las dos pilas con canciones
void probarPilas() {
    Album album = crearAlbum();

    std::cout << "=== Pila estatica ===" << std::endl;
    PilaEstatica<Cancion> estatica;
    for (int i = 0; i < 3; i++) estatica.push(album.canciones[i]);
    std::cout << "Tamaño: " << estatica.tamano() << " | Tope: " << estatica.peek().titulo << std::endl;
    try { estatica.push(album.canciones[3]); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << std::endl; }
    while (!estatica.estaVacia()) std::cout << "Pop: " << estatica.pop().titulo << std::endl;
    try { estatica.pop(); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << std::endl; }

    std::cout << "=== Pila dinamica ===" << std::endl;
    PilaDinamica<Cancion> dinamica;
    for (int i = 0; i < 3; i++) dinamica.push(album.canciones[i]);
    std::cout << "Tamaño: " << dinamica.tamano() << " | Tope: " << dinamica.peek().titulo << std::endl;
    dinamica.push(album.canciones[3]);
    std::cout << "Cuarta canción agregada, tamaño: " << dinamica.tamano() << std::endl;
    while (!dinamica.estaVacia()) std::cout << "Pop: " << dinamica.pop().titulo << std::endl;
    try { dinamica.pop(); }
    catch (const std::exception& e) { std::cout << "Error: " << e.what() << std::endl; }
}

// =====================================================
// PASO 5. Editor de playlist en C++ CREADO POR JORGE PAZ
// =====================================================

// 5.1 Mismos atributos y metodos del Paso 4
class EditorPlaylist {
private:
    std::vector<Cancion> playlist;      
    PilaEstatica<Accion> pilaDeshacer;  
    PilaDinamica<Accion> pilaRehacer;    

    // Vacia la pila de rehacer
    void vaciarRehacer() {
        while (!pilaRehacer.estaVacia()) pilaRehacer.pop();
    }

public:
    // Agrega una cancion al final y registra la accion
    void agregar(const Cancion& cancion) {
        if (pilaDeshacer.estaLlena()) {
            std::cout << "Historial lleno: guarda la playlist para seguir editando" << std::endl;
            return;
        }
        playlist.push_back(cancion);
        pilaDeshacer.push({"agregar", cancion, (int)playlist.size() - 1});
        vaciarRehacer();
    }

    // Elimina la cancion en la posicion (desde 1) y registra la accion
    void eliminar(int posicion) {
        if (pilaDeshacer.estaLlena()) {
            std::cout << "Historial lleno: guarda la playlist para seguir editando" << std::endl;
            return;
        }
        if (posicion < 1 || posicion > (int)playlist.size()) {
            std::cout << "Posición inválida" << std::endl;
            return;
        }
        int indice = posicion - 1;
        Cancion cancion = playlist[indice];
        playlist.erase(playlist.begin() + indice);
        pilaDeshacer.push({"eliminar", cancion, indice});
        vaciarRehacer();
    }

    // Revierte la ultima accion y la pasa a rehacer
    void deshacer() {
        if (pilaDeshacer.estaVacia()) {
            std::cout << "Nada que deshacer" << std::endl;
            return;
        }
        Accion a = pilaDeshacer.pop();
        if (a.tipo == "agregar") playlist.erase(playlist.begin() + a.posicion);
        else playlist.insert(playlist.begin() + a.posicion, a.cancion);
        pilaRehacer.push(a);
    }

    // Repite la ultima accion deshecha y la pasa a deshacer
    void rehacer() {
        if (pilaRehacer.estaVacia()) {
            std::cout << "Nada que rehacer" << std::endl;
            return;
        }
        Accion a = pilaRehacer.pop();
        if (a.tipo == "agregar") playlist.insert(playlist.begin() + a.posicion, a.cancion);
        else playlist.erase(playlist.begin() + a.posicion);
        pilaDeshacer.push(a);
    }

    // Vacia las dos pilas sin tocar la playlist
    void guardar() {
        while (!pilaDeshacer.estaVacia()) pilaDeshacer.pop();
        vaciarRehacer();
        std::cout << "Playlist guardada" << std::endl;
    }

    // Devuelve la playlist y el estado de las pilas en una linea
    std::string estado() const {
        std::ostringstream ss;
        ss << "[ ";
        for (size_t i = 0; i < playlist.size(); i++) {
            if (i > 0) ss << ", ";
            ss << playlist[i].titulo;
        }
        ss << " ] · Deshacer " << pilaDeshacer.tamano() << "/" << pilaDeshacer.capacidad()
           << " · Rehacer " << pilaRehacer.tamano();
        return ss.str();
    }

    // Muestra la playlist numerada y la duracion total
    void mostrar() const {
        int total = 0;
        for (size_t i = 0; i < playlist.size(); i++) {
            std::cout << i + 1 << ". " << playlist[i].formato() << std::endl;
            total += playlist[i].duracion;
        }
        std::cout << "Duracion total: " << formatoTiempo(total) << std::endl;
    }
};

// 5.2 Misma secuencia de 13 pasos del Paso 4.4
void paso5() {
    Album album = crearAlbum();
    EditorPlaylist editor;
    const Cancion* c = album.canciones;

    editor.agregar(c[0]);  std::cout << "Paso 1 · agregar cancion 1 → " << editor.estado() << std::endl;
    editor.agregar(c[1]);  std::cout << "Paso 2 · agregar cancion 2 → " << editor.estado() << std::endl;
    editor.agregar(c[2]);  std::cout << "Paso 3 · agregar cancion 3 → " << editor.estado() << std::endl;
    editor.agregar(c[3]);  std::cout << "Paso 4 · agregar cancion 4 → " << editor.estado() << std::endl;
    editor.deshacer();     std::cout << "Paso 5 · deshacer → " << editor.estado() << std::endl;
    editor.deshacer();     std::cout << "Paso 6 · deshacer → " << editor.estado() << std::endl;
    editor.rehacer();      std::cout << "Paso 7 · rehacer → " << editor.estado() << std::endl;
    editor.eliminar(1);    std::cout << "Paso 8 · eliminar(1) → " << editor.estado() << std::endl;
    editor.rehacer();      std::cout << "Paso 9 · rehacer → " << editor.estado() << std::endl;
    editor.deshacer();     std::cout << "Paso 10 · deshacer → " << editor.estado() << std::endl;
    editor.guardar();      std::cout << "Paso 11 · guardar → " << editor.estado() << std::endl;
    editor.deshacer();     std::cout << "Paso 12 · deshacer → " << editor.estado() << std::endl;
    editor.agregar(c[3]);  std::cout << "Paso 13 · agregar cancion 4 → " << editor.estado() << std::endl;

    std::cout << std::endl;
    editor.mostrar();
}

// =====================================================
// Programa principal
// =====================================================

int main() {
    paso1();
    std::cout << std::endl;
    probarPilas();
    std::cout << std::endl;
    paso5();
    return 0;
}