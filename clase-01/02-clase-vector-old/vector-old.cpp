// ============================================================
// vector-old.cpp
// Programación Aplicada al Sonido II — UNA
// Clase 1: La clase Vector del libro (Embree & Danieli, 1999)
// ============================================================
//
// Este archivo muestra cómo el libro implementa su propia clase
// Vector desde cero. Es C++ de 1999 — útil para entender de
// dónde vienen los conceptos, pero NO es cómo se hace hoy.
//
// Lo más importante para entender acá: LOS PUNTEROS.
// Un puntero es una variable que guarda la DIRECCIÓN de memoria
// donde vive otro dato — no el dato en sí.
//
// Ejemplo:
//   int valor = 42;          // "valor" guarda 42
//   int* ptr = &valor;       // "ptr" guarda la DIRECCIÓN de "valor"
//   cout << *ptr;            // el * "desreferencia" — imprime 42
//
// El libro usa punteros para manejar arrays de tamaño dinámico.
// En C++ moderno, std::vector hace esto automáticamente y de
// forma segura. Pero entender punteros ayuda a leer el libro
// y entender qué pasa "por debajo" de std::vector.
//
// IMPORTANTE: Este archivo NO compila solo — la clase Vector
// del libro depende de otros archivos (dsp.h, DSPException, etc.)
// que forman todo un ecosistema del libro. Está acá solo como
// referencia y material de estudio.
//
// Para el código que corre, ver: 03-clase-vector-modern/
// ============================================================


// ============================================================
// PARTE 1: LA CLASE VECTOR DEL LIBRO
// ============================================================

// El libro declara la clase como un template — puede guardar
// cualquier tipo de dato (float, int, double, etc.)
// En audio casi siempre se usa Vector<float>

template <class Type>
class Vector
{
private:
    // --------------------------------------------------------
    // DATOS INTERNOS — privados, nadie los toca desde afuera
    // --------------------------------------------------------

    // m_data es un PUNTERO a la memoria donde viven los datos.
    // No es el array en sí — es la dirección donde empieza.
    // La "m_" adelante es convención del libro para "member variable"
    Type *m_data;

    // La cantidad de elementos guardados
    int m_length;

    // --------------------------------------------------------
    // init() — función interna que reserva memoria
    // --------------------------------------------------------
    // Acá está el corazón del manejo manual de memoria.
    // "new Type[newLen]" le pide al sistema operativo que reserve
    // espacio para newLen elementos del tipo Type.
    // El sistema devuelve un puntero a ese espacio.
    void init( int newLen, const Type *newData = NULL )
    {
        empty(); // primero liberar memoria anterior si existe

        if( newLen <= 0 )
            return; // nada que hacer si el tamaño es 0 o negativo

        // Reservar memoria para newLen elementos
        // Esto es equivalente a lo que hace std::vector internamente
        m_data = new Type[ newLen ];

        // En C++ viejo, si new falla devuelve NULL
        // En C++ moderno lanza una excepción automáticamente
        if( m_data == NULL )
            return; // no había memoria disponible

        m_length = newLen;

        // Si nos pasaron datos iniciales, copiarlos
        if( newData != NULL )
        {
            // memcpy copia bytes crudos de memoria
            // Es rápido pero peligroso — no verifica tipos ni límites
            memcpy( m_data, newData, newLen * sizeof( Type ) );
        }
    }

public:
    // --------------------------------------------------------
    // CONSTRUCTORES — se llaman al crear el objeto
    // --------------------------------------------------------

    // Constructor por defecto y con tamaño
    Vector( int length = 0, const Type *newData = NULL ) :
        m_data( NULL ),   // inicializar puntero en NULL — importante!
        m_length( 0 )
    {
        init( length, newData );
    }

    // Copy constructor — se llama cuando hacés: Vector<float> b = a;
    // Tiene que copiar los DATOS, no solo el puntero.
    // Si copiara solo el puntero, ambos objetos apuntarían al mismo
    // lugar en memoria — al borrar uno, el otro quedaría inválido.
    Vector( const Vector<Type>& v ) :
        m_data( NULL ),
        m_length( 0 )
    {
        init( v.m_length, v.m_data ); // copia real de los datos
    }

    // --------------------------------------------------------
    // DESTRUCTOR — se llama automáticamente al destruir el objeto
    // --------------------------------------------------------
    // Este es el punto crítico del manejo manual de memoria.
    // Si no liberás la memoria con delete[], hay un "memory leak"
    // — la memoria queda ocupada para siempre hasta que el programa
    // termina. En audio en tiempo real esto es un problema grave.
    ~Vector() { empty(); }

    // --------------------------------------------------------
    // empty() — liberar memoria
    // --------------------------------------------------------
    void empty()
    {
        delete [] m_data;  // liberar la memoria reservada con new[]
        m_data = NULL;     // siempre poner en NULL después de delete
        m_length = 0;
    }

    // --------------------------------------------------------
    // INFORMACIÓN DEL VECTOR
    // --------------------------------------------------------
    bool isEmpty() const { return ( m_data == NULL ); }
    int length() const { return m_length; }

    // --------------------------------------------------------
    // OPERADOR [] — acceder a elementos
    // --------------------------------------------------------
    // Permite usar el vector como: v[0], v[1], etc.
    // Incluye validación de límites — si pedís un índice fuera
    // del rango, lanza una excepción en lugar de acceder a
    // memoria inválida (lo que causaría un crash silencioso)
    Type& operator[]( int element )
    {
        if( isEmpty() )
            return m_data[0]; // en el libro lanza excepción

        if( element < 0 || element >= m_length )
            return m_data[0]; // en el libro lanza excepción

        return m_data[element];
    }

    // --------------------------------------------------------
    // OPERADOR = — asignar otro vector
    // --------------------------------------------------------
    // Mismo problema que el copy constructor — hay que copiar
    // los datos, no el puntero.
    Vector<Type>& operator=( const Vector<Type>& v )
    {
        if( &v != this ) // evitar asignarse a sí mismo: a = a
            init( v.m_length, v.m_data );
        return *this;
    }

    // Asignar una constante a todos los elementos
    Vector<Type>& operator=( const Type t )
    {
        Type *pt = m_data; // puntero auxiliar para recorrer el array
        for( int i = 0; i < m_length; i++ )
            *pt++ = t; // *pt escribe en la dirección, luego avanza
        return *this;
    }
};


// ============================================================
// PARTE 2: OPERACIONES SOBRE VECTORES (funciones libres)
// ============================================================
// El libro define las operaciones matemáticas como funciones
// sueltas (no como métodos de la clase). Todas siguen el mismo
// patrón: reciben vectores, crean un vector resultado, lo llenan
// y lo devuelven.

// Sumar dos vectores elemento a elemento — mezclar dos señales
template <class Type>
Vector<Type> add( const Vector<Type>& v1, const Vector<Type>& v2 )
{
    // Usar el mínimo de los dos tamaños para no salirse de límites
    int len = v1.length() < v2.length() ? v1.length() : v2.length();

    Vector<Type> vRet( len ); // crear vector resultado
    for( int i = 0; i < len; i++ )
        vRet[i] = v1[i] + v2[i];
    return vRet;
}

// Restar dos vectores elemento a elemento
template <class Type>
Vector<Type> sub( const Vector<Type>& v1, const Vector<Type>& v2 )
{
    int len = v1.length() < v2.length() ? v1.length() : v2.length();
    Vector<Type> vRet( len );
    for( int i = 0; i < len; i++ )
        vRet[i] = v1[i] - v2[i];
    return vRet;
}

// Multiplicar dos vectores elemento a elemento
template <class Type>
Vector<Type> pwisemult( const Vector<Type>& v1, const Vector<Type>& v2 )
{
    int len = v1.length() < v2.length() ? v1.length() : v2.length();
    Vector<Type> vRet( len );
    for( int i = 0; i < len; i++ )
        vRet[i] = v1[i] * v2[i];
    return vRet;
}

// Dot product — suma de los productos elemento a elemento
// Devuelve un escalar, no un vector
// Es la base matemática de los filtros FIR
template <class Type>
Type mult( const Vector<Type>& v1, const Vector<Type>& v2 )
{
    int len = v1.length() < v2.length() ? v1.length() : v2.length();
    Type t = (Type)0;
    for( int i = 0; i < len; i++ )
        t += v1[i] * v2[i];
    return t;
}

// Suma de todos los elementos — devuelve un escalar
template <class Type>
Type sum( const Vector<Type>& v )
{
    Type t = (Type)0;
    for( int i = 0; i < v.length(); i++ )
        t += v[i];
    return t;
}

// Escalar todos los elementos por un float — subir/bajar volumen
template <class Type>
Vector<Type> scale( float s, const Vector<Type>& v )
{
    int len = v.length();
    Vector<Type> vRet( len );
    for( int i = 0; i < len; i++ )
        vRet[i] = (Type)(s * v[i]);
    return vRet;
}


// ============================================================
// RESUMEN: ¿QUÉ PROBLEMA RESUELVE ESTE CÓDIGO?
// ============================================================
//
// En C puro no existe un contenedor dinámico seguro.
// El libro construye Vector<Type> para tener:
//
//  1. Tamaño dinámico — no fijo en tiempo de compilación
//  2. Acceso seguro — validación de índices con []
//  3. Manejo automático de memoria — el destructor libera todo
//  4. Operaciones matemáticas — add, sub, scale, mult, etc.
//  5. Genericidad — funciona con float, int, double, etc.
//
// En C++ moderno (C++11 en adelante), std::vector ya hace
// todo esto y más, de forma más segura y eficiente.
// Ver 03-clase-vector-modern/ para la versión actual.
// ============================================================
