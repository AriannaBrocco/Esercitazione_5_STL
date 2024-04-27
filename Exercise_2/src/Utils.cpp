#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>


namespace PolygonalLibrary {
bool ImportaMesh(const string& nomeFile,
                PolygonalMesh& mesh)
{
    // Test sull'importazione delle celle 0D
    cout << "Test sulle celle 0D:" << endl;
    if(!ImportaCell0Ds(nomeFile + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto iter = mesh.Cell0DMarkers.begin(); iter != mesh.Cell0DMarkers.end(); iter++)
        {
            cout << "chiave:\t" << iter -> first << "\t valori:";
            for(const unsigned int id : iter -> second)
                cout << "\t" << id;

            cout << endl;
        }
        cout << endl;
    }

    // Test sull'importazione delle celle 1D
    cout << "Test sulle celle 1D:" << endl;
    if(!ImportaCell1Ds(nomeFile + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "chiave:\t" << it -> first << "\t valori:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
        cout << endl;
    }

    // Test sull'importazione delle celle 2D
    cout << "Test sulle celle 2D:" << endl;
    if(!ImportaCell2Ds(nomeFile + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    cout << "Corretta importazione delle celle 2D" << endl << endl;

    // Test sui lati
    cout << "Test sui lati: " << endl;
    if(!LunghezzaLati(mesh))
    {
        return false;
    }
    cout << "Nessun lato ha lunghezza nulla" << endl << endl;

    // Test sulle aree
    cout << "Test sulle aree:" << endl;
    if(!Area(mesh))
    {
        return false;
    }
    cout << "Non ci sono poligoni con area nulla" << endl << endl;

    return true;
}
// ***************************************************************************
bool ImportaCell0Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{
    // Legge il file
    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listaRighe;
    string riga;
    while (getline(file, riga))
        listaRighe.push_back(riga);

    file.close();

    listaRighe.pop_front();

    mesh.NumberCell0D = listaRighe.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "Non ci sono celle 0D" << endl;
        return false;
    }

    // Riserva spazio nella struttura Mesh per inserire Id e coordinate
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    // Per ogni riga legge i valori delle variabili inizializzate e li assegna ad esse eliminando il ; (sep)
    for (const string& riga : listaRighe)
    {
        istringstream converter(riga);
        char sep;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> sep >> marker >> sep >> coord(0) >> sep >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {

            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}
// ***************************************************************************
bool ImportaCell1Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{
    // Agisce allo stesso modo di ImportaCell0D
    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listaRighe;
    string riga;
    while (getline(file, riga))
        listaRighe.push_back(riga);

    listaRighe.pop_front();

    mesh.NumberCell1D = listaRighe.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "Non ci sono celle 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& riga : listaRighe)
    {
        istringstream converter(riga);
        char sep;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> sep >> marker >> sep >> vertices(0) >> sep >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {

            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}
// ***************************************************************************
bool ImportaCell2Ds(const string &nomeFile,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(nomeFile);

    if(file.fail())
        return false;

    list<string> listaRighe;
    string riga;
    while (getline(file, riga))
        listaRighe.push_back(riga);

    listaRighe.pop_front();

    mesh.NumberCell2D = listaRighe.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "Non ci sono celle 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& riga : listaRighe)
    {
        istringstream converter(riga);
        char sep;
        unsigned int id;
        unsigned int marker;
        unsigned int NumVertici;
        unsigned int NumLati;
        VectorXi vertici;
        VectorXi lati;

        // A differenza delle funzioni per le celle 0D e 1D qui è necessario utilizzare dei for per leggere i valori dei vertici e dei lati e creare dei vettori
        // I vettori hanno dimensione inizialmente ignota e poi vengono ridimensionati in base al valore contenuto nella variabile NumVertici e NumLati
        converter >> id >> sep >> marker >> sep >> NumVertici;
        vertici.resize(NumVertici);

        for(unsigned int i = 0; i < NumVertici; i++)
            converter >> sep >> vertici(i);

        converter >> sep >> NumLati;
        lati.resize(NumLati);

        for(unsigned int i = 0; i < NumLati; i++)
            converter >> sep >> lati(i);

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertici);
        mesh.Cell2DEdges.push_back(lati);
        if( marker != 0)
        {

            auto ret = mesh.Cell2DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();
    return true;
}

// ***************************************************************************
bool LunghezzaLati(PolygonalMesh& mesh)
{
    double tol = numeric_limits<double>::epsilon();
    // Il for scorre i vettori contenenti i vertici associati a ogni lato
    for(const auto& vertici : mesh.Cell1DVertices)
    {
        // Ricavate le coordinate dei due vertici associati a un certo lato, controlla se la distanza tra essi è minore di una certa tolleranza
        Vector2d coordinate1 = mesh.Cell0DCoordinates[vertici(0)];
        Vector2d coordinate2 = mesh.Cell0DCoordinates[vertici(1)];
        if(abs(coordinate1(0)-coordinate2(0)) < tol && abs(coordinate1(1)-coordinate2(1)) < tol)
        {
            cerr << "Alcuni lati hanno lunghezza nulla" << endl;
            return false;
        }
    }
    return true;
}

// ***************************************************************************
bool Area(PolygonalMesh& mesh)
{
    for(const auto& lati : mesh.Cell2DEdges)
    {
        // Ricava i vertici del primo lato e poi le loro coordinate
        Vector2i verticiLato = mesh.Cell1DVertices[lati(0)];
        Vector2d coordVertice_1 = mesh.Cell0DCoordinates[verticiLato(0)];
        Vector2d coordVertice_2 = mesh.Cell0DCoordinates[verticiLato(1)];

        // Calcola la pendenza e l'intercetta della retta passante per i due vertici del lato
        double coef_ang = (coordVertice_2[1]-coordVertice_1[1])/(coordVertice_2[0]-coordVertice_1[0]);
        double intercetta = coordVertice_2[0] - (coef_ang * coordVertice_1[0]);

        // Controlla se gli altri punti (vertici) sono allineati ai primi due
        bool su_stessa_retta = true;
        size_t num_lati = lati.size();
        for(size_t n = 2; n < num_lati; ++n)
        {
            Vector2i verticiLato_n = mesh.Cell1DVertices[lati(n)];
            Vector2d coordVertice_n = mesh.Cell0DCoordinates[verticiLato_n(0)];
            double y = coef_ang * coordVertice_n[0] + intercetta;

            if (coordVertice_n[1] != y)
            {
                su_stessa_retta = false;
                break;
            }
        }

        // Se i vertici del poligono sono allineati allora avremo un'area nulla
        if (su_stessa_retta)
        {
            cout << "Ci sono poligoni con area nulla" << endl;
        }

    }
    return true;
}



}
