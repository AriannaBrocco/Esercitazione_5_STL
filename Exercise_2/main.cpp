#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    string nomeFile = "PolygonalMesh";

    // Verifica che l'importo della mesh e tutti i test siano andati a buon fine
    if(!ImportaMesh(nomeFile,
                    mesh))
    {
        cerr << "Impossibile importare la mesh" << endl;
        return 1;
    }
    else
    {
        cout << "Mesh importata correttamente" << endl;
    }


    return 0;
}

