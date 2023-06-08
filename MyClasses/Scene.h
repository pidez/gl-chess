
/* Questa è la classe che dovrebbe gestire la scena nella sua interezza.
    Tutti gli oggetti che hanno in qualche modo effetto sulla scena devono essere gestiti tramite questa classe.
    Ad esempio la camera a cui si fa riferimento per il movimento deve essere la camera definita qui. (di conseguenza la
    posizione di camera passata agli shader sarà quella della camera contenuta in questa classe) */

#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "glm/glm.hpp"

#include "Mesh.h"
#include "Program.h"
#include "Camera.h"

class Scene {

public:

    Scene();
    Scene(std::vector<Mesh>& meshes, std::vector<Program>& programs);

    void setCamera(const Camera& camera);

    /* Si attiva la funzionalità di picking degli oggetti, dunque
    ogni operazione di rendering verrà eseguita due volte: una volta utilizzando
    un colore univoco per ogni mesh, l'altra con i colori originali. */
    void enablePicking();
    void disablePicking();

    /* Disegna tutte le mesh */
    void draw();


    /* Disegna le mesh specificate dal vettore di indici */
    void drawMeshes(std::vector<unsigned int> meshIndices) const;

    Camera& camera();




private:

    Camera _camera; ///< La camera posizionata nella scena

    /* Ogni Mesh è identificata dalla posizione all'interno dell'array */
    std::vector<Mesh> _meshes; ///< elenco delle mesh della scena
    
    std::vector<Program> _programs; ///< programmi di shader associati alle rispettive mesh (in ordine)

    /* Array utilizzato solo in caso di attivazione della funzionalità di picking */
    std::vector<glm::vec3> _colors; ///< colori univoci per ogni mesh

    bool _picking_enabled; ///< 
    int _picked_object; ///< ID dell'oggetto attualmente selezionato (Se ne esiste uno, valore negativo altrimenti)
};

#endif