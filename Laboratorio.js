// 1.1: recibe segundos (entero) y devuelve un texto con formato m:ss
function formatoTiempo(segundos) {
  const m = Math.floor(segundos / 60); // minutos completos
  const s = segundos % 60;             // segundos restantes
  return `${m}:${s.toString().padStart(2, "0")}`; // segundos siempre con 2 dígitos
}

//Paso 1: Tipos compuestos y agregados 

// 1.2: tipo compuesto que describe una canción
class Cancion {
  titulo;   // texto: nombre de la canción
  artista;  // texto: nombre del artista
  duracion; // entero: duración en segundos

  constructor(titulo, artista, duracion) {
    this.titulo = titulo;
    this.artista = artista;
    this.duracion = duracion;
  }

  // devuelve "Título - Artista (m:ss)"
  formato() {
    return `${this.titulo} - ${this.artista} (${formatoTiempo(this.duracion)})`;
  }
}

// 1.3: tipo compuesto que recuerda un cambio hecho en la playlist
class Accion {
  tipo;     // texto: "agregar" o "eliminar"
  cancion;  // Cancion completa afectada por el cambio
  posicion; // entero: índice de la canción en la playlist, desde 0

  constructor(tipo, cancion, posicion) {
    this.tipo = tipo;
    this.cancion = cancion;
    this.posicion = posicion;
  }
}

// 1.4: tipo compuesto que agrupa las canciones del álbum (un agregado)
class Album {
  titulo;    // texto: nombre del álbum
  anio;      // entero: año de lanzamiento
  canciones; // arreglo de 4 Cancion (el agregado)

  constructor(titulo, anio, canciones) {
    this.titulo = titulo;
    this.anio = anio;
    this.canciones = canciones;
  }

  // devuelve cuántas canciones tiene el álbum
  cantidad() {
    return this.canciones.length;
  }

  // devuelve la suma de las duraciones, en segundos, con un ciclo
  duracionTotal() {
    let total = 0;
    for (const c of this.canciones) total += c.duracion;
    return total;
  }
}

// 1.5: imprime el álbum en el formato pedido
function imprimirAlbum(album) {
  console.log(`${album.titulo} (${album.anio}) · ${album.cantidad()} canciones`);
  album.canciones.forEach((c, i) => {
    console.log(`${i + 1}. ${c.formato()}`);
  });
  console.log(`Duración del álbum: ${formatoTiempo(album.duracionTotal())}`);
}

//Paso 2: Pila estática 

// 2.1: TDA Pila con capacidad fija (histórico de Deshacer)
class PilaEstatica {
  #datos;        // arreglo de tamaño fijo usado como almacenamiento
  #tope;         // entero: cuántos elementos hay ahora mismo (próximo índice libre)
  #capacidadMax; // entero: tamaño máximo de la pila

  constructor(capacidad) {
    this.#capacidadMax = capacidad;
    this.#datos = new Array(capacidad);
    this.#tope = 0;
  }

  // mete un elemento en el tope; lanza error si la pila está llena
  push(elemento) {
    if (this.estaLlena()) {
      throw new Error("Pila llena: no cabe otro elemento");
    }
    this.#datos[this.#tope] = elemento;
    this.#tope++;
  }

  // saca y devuelve el elemento del tope; lanza error si está vacía
  pop() {
    if (this.estaVacia()) {
      throw new Error("Pila vacía: no hay nada que sacar");
    }
    this.#tope--;
    const elemento = this.#datos[this.#tope];
    this.#datos[this.#tope] = undefined; // limpia la casilla que libera
    return elemento;
  }

  // devuelve el elemento del tope sin sacarlo
  peek() {
    if (this.estaVacia()) {
      return undefined; // en JS, peek con la pila vacía devuelve undefined
    }
    return this.#datos[this.#tope - 1];
  }

  // true si no hay elementos
  estaVacia() {
    return this.#tope === 0;
  }

  // true si ya se llegó a la capacidad máxima
  estaLlena() {
    return this.#tope === this.#capacidadMax;
  }

  // cuántos elementos hay ahora (depende del tope, no del largo del arreglo)
  tamano() {
    return this.#tope;
  }

  // el tamaño máximo que puede tener la pila
  capacidad() {
    return this.#capacidadMax;
  }
}

//Paso 3: Pila dinámica

// 3.1: TDA Pila sin límite de capacidad (histórico de Rehacer)
class PilaDinamica {
  #datos; // arreglo que crece y encoge; el final es el tope

  constructor() {
    this.#datos = [];
  }

  // mete un elemento al final (tope) del arreglo
  push(elemento) {
    this.#datos.push(elemento);
  }

  // saca y devuelve el último elemento; lanza error si está vacía
  pop() {
    if (this.estaVacia()) {
      throw new Error("Pila vacía: no hay nada que sacar");
    }
    return this.#datos.pop();
  }

  // devuelve el último elemento sin sacarlo
  peek() {
    if (this.estaVacia()) {
      return undefined;
    }
    return this.#datos[this.#datos.length - 1];
  }

  // true si no tiene elementos
  estaVacia() {
    return this.#datos.length === 0;
  }

  // cuántos elementos tiene ahora mismo
  tamano() {
    return this.#datos.length;
  }
}

// 2.4 + 3.3: una sola función que prueba las dos pilas
function probarPilas(album) {
  console.log("=== Prueba PilaEstatica<Cancion> (capacidad 3) ===");
  const estatica = new PilaEstatica(3);
  const [c1, c2, c3, c4] = album.canciones;

  // 1. mete las tres primeras canciones
  [c1, c2, c3].forEach((c) => {
    estatica.push(c);
    console.log(`tamaño: ${estatica.tamano()}, tope: ${estatica.peek().titulo}`);
  });

  // 2. intenta meter la cuarta (pila llena)
  try {
    estatica.push(c4);
  } catch (e) {
    console.log(e.message);
  }

  // 3. saca todas con while (!estaVacia())
  while (!estatica.estaVacia()) {
    console.log(estatica.pop().titulo);
  }

  // 4. intenta sacar una más (pila vacía)
  try {
    estatica.pop();
  } catch (e) {
    console.log(e.message);
  }

  console.log("\n=== Prueba PilaDinamica<Cancion> (sin límite) ===");
  const dinamica = new PilaDinamica();

  // mete las cuatro canciones; esta vez sí entra la cuarta
  album.canciones.forEach((c) => {
    dinamica.push(c);
    console.log(`tamaño: ${dinamica.tamano()}, tope: ${dinamica.peek().titulo}`);
  });

  while (!dinamica.estaVacia()) {
    console.log(dinamica.pop().titulo);
  }

  try {
    dinamica.pop();
  } catch (e) {
    console.log(e.message);
  }
}

//Paso 4: Editor de playlist 

// 4.1: EditorPlaylist con tres atributos privados
class EditorPlaylist {
  #playlist; // Arreglo de Cancion, empieza Vacío
  #deshacer; // PilaEstatica de Accion, capacidad 3, empieza Vacía
  #rehacer;  // PilaDinamica de Accion, empieza Vacía

  constructor() {
    this.#playlist = [];
    this.#deshacer = new PilaEstatica(3);
    this.#rehacer = new PilaDinamica();
  }

  // 4.2
  agregar(cancion) {
    if (this.#deshacer.estaLlena()) {
      console.log("Historial lleno: guarda la playlist para seguir editando");
      return;
    }
    const posicion = this.#playlist.length;
    this.#playlist.push(cancion);
    this.#deshacer.push(new Accion("agregar", cancion, posicion));
    this.#vaciarRehacer();
  }

  eliminar(posicion) {
    // la posición empieza en 1
    if (this.#deshacer.estaLlena()) {
      console.log("Historial lleno: guarda la playlist para seguir editando");
      return;
    }
    const indice = posicion - 1;
    if (indice < 0 || indice >= this.#playlist.length) {
      console.log("Posición inválida");
      return;
    }
    const [cancion] = this.#playlist.splice(indice, 1);
    this.#deshacer.push(new Accion("eliminar", cancion, indice));
    this.#vaciarRehacer();
  }

  deshacer() {
    if (this.#deshacer.estaVacia()) {
      console.log("Nada que deshacer");
      return;
    }
    const accion = this.#deshacer.pop();
    if (accion.tipo === "agregar") {
      this.#playlist.splice(accion.posicion, 1);
    } else {
      this.#playlist.splice(accion.posicion, 0, accion.cancion);
    }
    this.#rehacer.push(accion);
  }

  rehacer() {
    if (this.#rehacer.estaVacia()) {
      console.log("Nada que rehacer");
      return;
    }
    const accion = this.#rehacer.pop();
    if (accion.tipo === "agregar") {
      this.#playlist.splice(accion.posicion, 0, accion.cancion);
    } else {
      this.#playlist.splice(accion.posicion, 1);
    }
    this.#deshacer.push(accion);
  }

  guardar() {
    while (!this.#deshacer.estaVacia()) this.#deshacer.pop();
    this.#vaciarRehacer();
    console.log("Playlist guardada");
  }

  estado() {
    const titulos = this.#playlist.map((c) => c.titulo);
    return `[ ${titulos.join(", ")} ] · Deshacer ${this.#deshacer.tamano()}/${this.#deshacer.capacidad()} · Rehacer ${this.#rehacer.tamano()}`;
  }

  mostrar() {
    console.log("--- Playlist actual ---");
    this.#playlist.forEach((c, i) => console.log(`${i + 1}. ${c.formato()}`));
    const total = this.#playlist.reduce((acc, c) => acc + c.duracion, 0);
    console.log(`Duración total: ${formatoTiempo(total)}`);
  }

  #vaciarRehacer() {
    while (!this.#rehacer.estaVacia()) this.#rehacer.pop();
  }
}

//Programa principal

// Regla 6: cuatro canciones reales, con su duración real
const album = new Album("Lo Mejor de la Música Ranchera y Grupera", 2024, [
  new Cancion("El Rey", "Vicente Fernández", 174),            // 2:54
  new Cancion("La Puerta Negra", "Los Tigres del Norte", 189),// 3:09
  new Cancion("Tu Cárcel", "Los Bukis", 245),                 // 4:05
  new Cancion("Por Qué Te Conocí", "Los Temerarios", 290),    // 4:50
]);

// 1.5
console.log("=== Paso 1.5: Álbum ===");
imprimirAlbum(album);
console.log();

// 2.4 + 3.3
probarPilas(album);
console.log();

// 4.4: secuencia de 13 pasos con las canciones del álbum
console.log("=== Paso 4.4: Secuencia EditorPlaylist ===");
const editor = new EditorPlaylist();
const [c1, c2, c3, c4] = album.canciones;

function paso(n, descripcion, accion) {
  accion();
  console.log(`Paso ${n} · ${descripcion} → ${editor.estado()}`);
}

paso(1, "agregar canción 1 del álbum", () => editor.agregar(c1));
paso(2, "agregar canción 2", () => editor.agregar(c2));
paso(3, "agregar canción 3", () => editor.agregar(c3));
paso(4, "agregar canción 4", () => editor.agregar(c4));
paso(5, "deshacer", () => editor.deshacer());
paso(6, "deshacer", () => editor.deshacer());
paso(7, "rehacer", () => editor.rehacer());
paso(8, "eliminar(1)", () => editor.eliminar(1));
paso(9, "rehacer", () => editor.rehacer());
paso(10, "deshacer", () => editor.deshacer());
paso(11, "guardar", () => editor.guardar());
paso(12, "deshacer", () => editor.deshacer());
paso(13, "agregar canción 4", () => editor.agregar(c4));

console.log();
editor.mostrar();