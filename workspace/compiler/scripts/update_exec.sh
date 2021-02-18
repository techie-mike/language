#!/bin/bash
# Update file frontend
cp ../../../frontend/cmake-build-debug/frontend ./..

# Update file frontend_back
cp ../../../frontend_back/cmake-build-debug/frontend_back ./..

# Update file backend
cp ../../../backend/cmake-build-debug/backend ./..

# Update file middleend
cp ../../../middle_end/cmake-build-debug/middle_end ./..