APPNAME='umeshu'
VERSION='0.1'

top = '.'
out = '../../../Builds/umeshu.build/'

sources = """
    src/box2.c
    src/edge.c
    src/element.c
    src/mesh.c
    src/mesh_delaunay.c
    src/mesh_output.c
    src/mesh_refine.c
    src/point2.c
    src/polygon.c
    src/polygon_triangulate.c
    src/predicates.c
    src/node.c
    predicates_init.h
""".split()

defines = """
    GL_GLEXT_PROTOTYPES
""".split()

includes = """
    src/core
    include/config
""".split()


def options(opt):
    opt.tool_options('compiler_c')

def configure(conf):
    print('→ configuring the project')
    conf.check_tool('compiler_c')

    # check the glib library
    conf.check_cfg( package='glib', uselib_store='glib', args='--cflags --libs', mandatory=True )

    # compilation flags
    conf.env.CFLAGS = [ '-Wall', '-msse2' ]

