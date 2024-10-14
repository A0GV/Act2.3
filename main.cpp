#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

// Estructura para almacenar los detalles de una entrada de registro
struct Registro {
    string mes;
    int dia;
    string hora;
    string ip;
    string razon;
    Registro *siguiente;
    Registro *anterior;
};

class Lista {
private:
    Registro *cabeza;
    Registro *cola;

public:
    Lista() : cabeza(nullptr), cola(nullptr) {}

    // Función para agregar una nueva entrada de registro a la lista
    // Complejidad: O(1)
    void agregarRegistro(const string &mes, int dia, const string &hora, const string &ip, const string &razon) {
        Registro *nuevo = new Registro{mes, dia, hora, ip, razon, nullptr, nullptr};
        if (!cabeza) {
            cabeza = cola = nuevo;
        } else {
            cola->siguiente = nuevo;
            nuevo->anterior = cola;
            cola = nuevo;
        }
    }

    // Función para ordenar la lista por direcciones IP usando merge sort
    // Complejidad: O(n log n)
    void ordenarPorIP() {
        if (!cabeza || !cabeza->siguiente) return;
        cabeza = mergeSort(cabeza);
        Registro *temp = cabeza;
        while (temp->siguiente) {
            temp = temp->siguiente;
        }
        cola = temp;
    }

    // Función para encontrar la primera entrada de registro dentro del rango de IP
    // Complejidad: O(n)
    Registro* buscarPorRangoIP(const string &ipInicio, const string &ipFin) {
        Registro *temp = cabeza;
        while (temp && temp->ip < ipInicio) {
            temp = temp->siguiente;
        }
        return temp;
    }

    // Función para imprimir las entradas de registro dentro del rango de IP
    // Complejidad: O(m)
    void imprimirPorRangoIP(const string &ipInicio, const string &ipFin) {
        Registro *temp = buscarPorRangoIP(ipInicio, ipFin);
        while (temp && temp->ip <= ipFin) {
            cout << temp->mes << " " << temp->dia << " " << temp->hora << " " << temp->ip << " " << temp->razon << endl;
            temp = temp->siguiente;
        }
    }

private:
    // Función de merge sort para ordenar la lista
    // Complejidad: O(n log n)
    Registro* mergeSort(Registro* head) {
        if (!head || !head->siguiente) return head;
        Registro *middle = getMiddle(head);
        Registro *half = middle->siguiente;
        middle->siguiente = nullptr;
        if (half) half->anterior = nullptr;
        return merge(mergeSort(head), mergeSort(half));
    }

    // Función para encontrar el medio de la lista
    // Complejidad: O(n)
    Registro* getMiddle(Registro* head) {
        if (!head) return head;
        Registro *slow = head, *fast = head->siguiente;
        while (fast && fast->siguiente) {
            slow = slow->siguiente;
            fast = fast->siguiente->siguiente;
        }
        return slow;
    }

    // Función de merge para fusionar dos mitades ordenadas
    // Complejidad: O(n)
    Registro* merge(Registro* left, Registro* right) {
        if (!left) return right;
        if (!right) return left;
        if (left->ip <= right->ip) {
            left->siguiente = merge(left->siguiente, right);
            if (left->siguiente) left->siguiente->anterior = left;
            left->anterior = nullptr;
            return left;
        } else {
            right->siguiente = merge(left, right->siguiente);
            if (right->siguiente) right->siguiente->anterior = right;
            right->anterior = nullptr;
            return right;
        }
    }
};

int main() {
    ifstream archivo("C:\\Users\\adolf\\Desktop\\UNI\\Semestre 3\\Programacion de esreucturas de datos y algoritos\\Act_2.3\\bitacora-1.txt");
    string linea, mes, hora, ip, razon;
    int dia;

    Lista bitacora;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            istringstream iss(linea);
            iss >> mes >> dia >> hora >> ip;
            getline(iss, razon);
            bitacora.agregarRegistro(mes, dia, hora, ip, razon);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo" << endl;
        return 1;
    }

    bitacora.ordenarPorIP();

    string ipInicio, ipFin;
    cout << "Ingrese la IP de inicio de busqueda: ";
    cin >> ipInicio;
    cout << "Ingrese la IP de fin de busqueda: ";
    cin >> ipFin;

    bitacora.imprimirPorRangoIP(ipInicio, ipFin);

    return 0;
}