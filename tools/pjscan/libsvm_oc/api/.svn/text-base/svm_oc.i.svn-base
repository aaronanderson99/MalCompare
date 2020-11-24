# Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
# 
# This file is part of libsvm_oc.
#
# libsvm_oc is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libsvm_oc is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libsvm_oc.  If not, see <http://www.gnu.org/licenses/>.

%module svm_oc

%{
#define SWIG_FILE_WITH_INIT
#include "../src/SvmOc.h"
%}

namespace svm_oc {

class SvmOc {
public:
   SvmOc(double nu, stem::Kernel *kernel);
   virtual ~SvmOc();
   SvmOc(const SvmOc &other);

   void learn(stem::NGram *ngram_array, unsigned int size);
   double getDistance(stem::NGram ngram);

   void setVerbosity(unsigned int verbosity) {
      verbose = verbosity;
   }
   double getRadius() const;
   stem::NGram getCenter() const {
      return center;
   }
};

}
