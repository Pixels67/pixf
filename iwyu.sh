cmake -DCMAKE_DISABLE_PRECOMPILE_HEADERS=ON \
      -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=iwyu \
      -B cmake-build-iwyu .

iwyu_tool.py -p cmake-build-iwyu/ Core/src/ -j 8 2>&1 | tee iwyu_output.txt