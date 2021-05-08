# -- Project information -----------------------------------------------------

project = 'Tristeon'
copyright = '2019-2021, Leon Brands, Tristan Metz'
author = 'Leon Brands, Tristan Metz'

# The full version, including alpha/beta/rc tags
release = '0.0.1-Alpha'

# Breathe Configuration
breathe_projects = { "Tristeon": "../xml" }
breathe_default_project = "Tristeon"
breathe_default_members = ('members', 'undoc-members')

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.intersphinx',
    'sphinx.ext.autosectionlabel',
    'sphinx.ext.todo',
    'sphinx.ext.coverage',
    'sphinx.ext.mathjax',
    'sphinx.ext.ifconfig',
    'sphinx.ext.viewcode',
    'sphinx.ext.inheritance_diagram',
     "breathe",
     "sphinx_rtd_theme"
]

language = 'English'
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']
highlight_language = 'c++'

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']

html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'navigation_depth': 5,
    'prev_next_buttons_location': None
}