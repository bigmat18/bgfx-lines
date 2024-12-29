#include <vclib/glfw/viewer_window.h>

#include <vclib/algorithms/mesh/update/color.h>
#include <vclib/algorithms/mesh/update/normal.h>
#include <vclib/load_save.h>
#include <vclib/meshes/tri_mesh.h>

#include <vclib/render/drawable/drawable_mesh.h>

int main(int argc, char** argv)
{
    vcl::glfw::ViewerWindow tw("Viewer GLFW");

    // load and set up a drawable mesh
    vcl::TriMesh m = vcl::load<vcl::TriMesh>("bimba.obj");
    vcl::updatePerVertexAndFaceNormals(m);

    // enable the vertex color of the mesh and set it to gray
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::Gray);

    // create a MeshRenderSettings object, that allows to set the rendering
    // options of the mesh
    // default is what we want: color per vertex, smooth shading, no wireframe
    vcl::MeshRenderSettings settings(m);

    // create a DrawableMesh object from the mesh
    vcl::DrawableMesh<vcl::TriMesh> drawable(m);

    // set the settings to the drawable mesh
    drawable.setRenderSettings(settings);

    // add the drawable mesh to the scene
    // the viewer will own **a copy** of the drawable mesh
    tw.pushDrawableObject(drawable);

    tw.fitScene();

    tw.show();

    return 0;
}