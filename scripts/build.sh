#!/bin/bash
# This script builds the Edge Quest web application.

# Create a distribution directory if it doesn't exist
mkdir -p dist

# Copy HTML and CSS files to the distribution directory
cp web/index.html dist/
cp web/style.css dist/

# Copy JavaScript files to the distribution directory
cp web/game.js dist/

# Optionally, you can add any build steps here, such as minification or bundling
# For example, using a tool like UglifyJS or Webpack

echo "Build completed successfully."