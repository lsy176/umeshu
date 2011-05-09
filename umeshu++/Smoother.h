#ifndef __SMOOTHER_H_INCLUDED__
#define __SMOOTHER_H_INCLUDED__ 

namespace umeshu {

template<typename Mesh>
class Smoother
{
public:
    typedef typename Mesh::kernel_type kernel_type;

    void smooth(Mesh &mesh, int niter);

private:
    void smooth_once(Mesh &mesh);
};

template<typename Mesh>
void Smoother<Mesh>::smooth(Mesh &mesh, int niter)
{
    for (int i = 0; i < niter; ++i) {
        this->smooth_once(mesh);
    }
}

template<typename Mesh>
void Smoother<Mesh>::smooth_once(Mesh &mesh)
{
    typename Mesh::nodes_iterator nodes_iter = mesh.nodes_begin();
    for(; nodes_iter != mesh.nodes_end(); ++nodes_iter) {
        NodeHandle node = *nodes_iter;
        if (node->is_boundary()) {
            continue;
        }
        Point2 new_pos;
        int n = node->degree();
        HalfEdgeHandle he_start = node->out_he()->pair();
        HalfEdgeHandle he_iter = he_start;
        do {
            new_pos += he_iter->origin()->position()/n;
            he_iter = he_iter->next()->pair();
        } while (he_iter != he_start);
        node->position() = new_pos;
    }
}

} // namespace umeshu

#endif /* __SMOOTHER_H_INCLUDED__ */
