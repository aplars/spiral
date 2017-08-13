TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = code/adt code/config code/math code/scene_models code/renderer_engine code/scene_engine sandbox \
    code/game_engine \
    geodesy \
    code/spatial \
    code/scene_editor
SUBDIRS += tools/meshimporter

code/scene_engine.depends = code/adt code/math code/scene_models code/renderer_engine
tools/meshimporter.depends = code/config code/math code/scene_models
sandbox.depends = code/adt code/math code/scene_models code/renderer_engine code/scene_engine


