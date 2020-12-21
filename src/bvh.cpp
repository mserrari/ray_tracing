
#include "bvh.h"
#include "mesh.h"
#include <iostream>

void BVH::build(const Mesh* pMesh, int targetCellSize, int maxDepth)
{
    // store a pointer to the mesh
    m_pMesh = pMesh;
    // allocate the root node
    m_nodes.resize(1);

    if(m_pMesh->nbFaces() <= targetCellSize) { // only one node
        m_nodes[0].box = pMesh->AABB();
        m_nodes[0].first_face_id = 0;
        m_nodes[0].is_leaf = true;
        m_nodes[0].nb_faces = m_pMesh->nbFaces();
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_faces[i] = i;
        }
    }else{
        // reserve space for other nodes to avoid multiple memory reallocations
        m_nodes.reserve( std::min<int>(2<<maxDepth, std::log(m_pMesh->nbFaces()/targetCellSize) ) );

        // compute centroids and initialize the face list
        m_centroids.resize(m_pMesh->nbFaces());
        m_faces.resize(m_pMesh->nbFaces());
        for(int i=0; i<m_pMesh->nbFaces(); ++i)
        {
            m_centroids[i] = (m_pMesh->vertexOfFace(i, 0).position + m_pMesh->vertexOfFace(i, 1).position + m_pMesh->vertexOfFace(i, 2).position)/3.f;
            m_faces[i] = i;
        }

        // recursively build the BVH, starting from the root node and the entire list of faces
        buildNode(0, 0, m_pMesh->nbFaces(), 0, targetCellSize, maxDepth);
    }
}

bool BVH::intersect(const Ray& ray, Hit& hit) const
{
    // compute the intersection with the root node
    float tMin, tMax;
    Normal3f n;
    bool intersect_b = ::intersect(ray, m_nodes[0].box, tMin, tMax, n);

    // TODO
    // vérifier si on a bien une intersection (en fonction de tMin, tMax, et hit.t()), et si oui appeler intersecNode...
    if (intersect_b && tMin <= hit.t()) { 
        return intersectNode(0, ray, hit);
    }

    return false;
}



bool BVH::intersectNode(int nodeId, const Ray& ray, Hit& hit) const
{
    // TODO, deux cas: soit mNodes[nodeId] est une feuille (il faut alors intersecter les triangles du noeud),
    // soit c'est un noeud interne (il faut visiter les fils (ou pas))
    bool result = false;
    const Node& node = m_nodes[nodeId];

    // deux cas:
    if (node.is_leaf) {
        for (int i = node.first_face_id; i < node.first_face_id + node.nb_faces; i++) {
            result |= m_pMesh->intersectFace(ray, hit, m_faces[i]);
        }
    }
    else {

        float l_tMin, l_tMax, r_tMin, r_tMax;
        Normal3f l_n, r_n;
        bool l_bool = ::intersect(ray, m_nodes[node.first_child_id].box, l_tMin, l_tMax, l_n);
        bool r_bool = ::intersect(ray, m_nodes[node.first_child_id + 1].box, r_tMin, r_tMax, l_n);

        // No intersection with either child nodes
        if (!l_bool && !r_bool) {
            result = false;
        }
        // Intersection only with left node
        else if (l_bool && !r_bool) {
            result = intersectNode(node.first_child_id, ray, hit);
        }
        // Intersection only with right node
        else if (!l_bool && r_bool) {
            result = intersectNode(node.first_child_id + 1, ray, hit);
        }
        // Intersection with both nodes, we have to detemine which node is in front
        else {
            result = intersectNode(node.first_child_id, ray, hit);
            if (l_tMax > r_tMin || hit.t() > r_tMin || !result) {
                bool tmp = intersectNode(node.first_child_id + 1, ray, hit);
                // If we found an intersection in the right box, return the new value
                result = tmp ? tmp : result;
            }
        }
    }
    return result;
}

/** Sorts the faces with respect to their centroid along the dimension \a dim and spliting value \a split_value.
  * \returns the middle index
  */
int BVH::split(int start, int end, int dim, float split_value)
{
    int l(start), r(end-1);
    while(l<r)
    {
        // find the first on the left
        while(l<end && m_centroids[l](dim) < split_value) ++l;
        while(r>=start && m_centroids[r](dim) >= split_value) --r;
        if(l>r) break;
        std::swap(m_centroids[l], m_centroids[r]);
        std::swap(m_faces[l], m_faces[r]);
        ++l;
        --r;
    }
    return m_centroids[l][dim]<split_value ? l+1 : l;
}

void BVH::buildNode(int nodeId, int start, int end, int level, int targetCellSize, int maxDepth)
{
    Node& node = m_nodes[nodeId];

    // étape 1 : calculer la boite englobante des faces indexées de m_faces[start] à m_faces[end]

    Eigen::AlignedBox3f box;
    for (int i = start; i < end; i++) {
        box.extend(m_pMesh->vertexOfFace(m_faces[i], 0).position);
        box.extend(m_pMesh->vertexOfFace(m_faces[i], 1).position);
        box.extend(m_pMesh->vertexOfFace(m_faces[i], 2).position);
    }

    node.box = box;
    node.nb_faces = end - start;
    // étape 2 : déterminer si il s'agit d'une feuille (appliquer les critères d'arrêts)
    // Si c'est une feuille, finaliser le noeud et quitter la fonction
    if (level >= ( maxDepth - 1 ) || end - start < targetCellSize) {
        node.first_face_id = start;
        node.is_leaf = true;
        return;
    }
    // Si c'est un noeud interne :
    // étape 3 : calculer l'index de la dimension (x=0, y=1, ou z=2) et la valeur du plan de coupe
    // (on découpe au milieu de la boite selon la plus grande dimension)
    Vector3f sizes = box.max() - box.min();
    Vector3f centers = (box.max() + box.min()) / 2.0f;

    int splitDim = 0;
    splitDim = sizes[1] > sizes[splitDim] ? 1 : splitDim;
    splitDim = sizes[2] > sizes[splitDim] ? 2 : splitDim;

    float splitVal = centers[splitDim];

    // étape 4 : appeler la fonction split pour trier (partiellement) les faces et vérifier si le split a été utile
    int splitValue = split(start, end, splitDim, splitVal);

    // test des cas limites
    if (splitValue == start || splitValue == end) {
        node.first_face_id = start;
        node.is_leaf = true;
        return;
    }
    // étape 5 : allouer les fils, et les construire en appelant buildNode...

    int size = m_nodes.size();
    node.first_child_id = size;
    m_nodes.resize(m_nodes.size() + 2);
    buildNode(size, start, splitValue, level + 1, targetCellSize, maxDepth);
    buildNode(size + 1, splitValue, end, level + 1, targetCellSize, maxDepth);
}