# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-src"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-build"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/tmp"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/src/gtest_pack-populate-stamp"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/src"
  "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/src/gtest_pack-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/src/gtest_pack-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/romolog/Documents/CPP/00_Portfolio/Portfolio_Vector_UnderTheHood/build/_deps/gtest_pack-subbuild/gtest_pack-populate-prefix/src/gtest_pack-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
