project('zad_2', 'cpp',
	default_options: [
		'cpp_std=c++2a',
		'optimization=2',
		'warning_level=3',
		'werror=true',
	]
)

gnome = import('gnome')

gtkmm = dependency('gtkmm-3.0')

add_project_arguments('-fanalyzer', language : 'cpp')

sources = files('main.cpp')

gresources = gnome.compile_resources('gresources', 'main.gresource.xml')

executable(meson.project_name(), sources, gresources,
	dependencies: gtkmm, install: true)
