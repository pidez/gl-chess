#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Mesh.h"
#include "Program.h"

class Scene {

public:

    Scene();
    Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs);

    /* Si attiva la funzionalità di picking degli oggetti, dunque
    ogni operazione di rendering verrà eseguita due volte: una volta utilizzando
    un colore univoco per ogni mesh, l'altra con i colori originali. */
    void enablePicking();
    void disablePicking();

    /* Disegna tutte le mesh */
    void draw();


    /* Disegna le mesh specificate dal vettore di indici */
    void drawMeshes(std::vector<unsigned int> meshIndices) const;




private:


    std::vector<Mesh> _meshes; ///< elenco delle mesh della scena
    std::vector<Program> _programs; ///< programmi di shader associati alle rispettive mesh (in ordine)

    bool _picking_enabled; ///< 
    int _picked_object; ///< ID dell'oggetto attualmente selezionato (Se ne esiste uno, valore negativo altrimenti)
};

#endif