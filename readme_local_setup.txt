IPCEI NXP — Lecții FRDM-MCXA153
Local development setup for the docs site (Jekyll)

============================================================
Prerequisites
============================================================
1. Install Ruby  https://rubyinstaller.org/  (Windows)
   -> Select "Add Ruby executables to your PATH"
   -> Select "MSYS2 and MINGW development toolchain"

============================================================
First-time setup
============================================================
cd docs
bundle install

============================================================
Run local server
============================================================
cd docs
bundle exec jekyll serve

-> Open http://localhost:4000/ in your browser
-> The site reloads automatically when you edit markdown files
-> Press Ctrl+C to stop the server

============================================================
Notes
============================================================
- Portfolio page  : docs/index.html
- Lesson files    : docs/lessons/*.md
- Lesson layout   : docs/_layouts/lesson.html
- Syntax CSS      : docs/assets/css/syntax.css

To add a new lesson:
  1. Create docs/lessons/lX-name.md
  2. Add front matter:
       ---
       title: "LX — Title"
       description: "Short description"
       nav_order: X
       layout: lesson
       ---

After updating Gemfile, run: bundle install
