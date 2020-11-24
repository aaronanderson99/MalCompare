# Copyright 2012 Nedim Srndic, Pavel Laskov, University of Tuebingen
# 
# This file is part of libstem.
#
# libstem is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# libstem is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with libstem.  If not, see <http://www.gnu.org/licenses/>.

%module stem

%{
#define SWIG_FILE_WITH_INIT
#include "../src/NGramConf.h"
#include "../src/NGramItem.h"
#include "../src/NGram.h"
#include "../src/Kernel.h"
%}

namespace stem {

// Supported embedding types
enum EmbeddingType {
   COUNT,
   BINARY,
   FREQUENCY,
   NORMALIZED,
   BINARY_NORMALIZED
};

// Number of supported embedding types
static const unsigned int NUM_EMBEDDINGS = 5;

class NGramConf {
public:
   NGramConf(EmbeddingType type, unsigned int length);

   // Getters
   EmbeddingType getEmbeddingType() const {
      return type;
   }
   unsigned int getLength() const {
      return length;
   }

   // Comparison
   bool operator ==(const NGramConf &other) const {
      return type == other.type && length == other.length;
   }
};

class NGramItem {
public:
   NGramItem() {
   }
   NGramItem(unsigned long feature, double value) :
      feature(feature), value(value) {
   }

   // Getters
   unsigned long getFeature() const {
      return feature;
   }
   double getValue() const {
      return value;
   }
   // Setters
   void setFeature(unsigned long newFeature) {
      feature = newFeature;
   }
   void setValue(double newValue) {
      value = newValue;
   }

   // Comparison operators
   bool operator <(const NGramItem &other) const {
      return feature < other.feature;
   }
   bool operator ==(const NGramItem &other) const {
      return feature == other.feature;
   }
   // Returns true if the value of the first item is smaller than the value of
   // the second item
   static bool compare_values(const NGramItem &a, const NGramItem &b) {
      return a.value < b.value;
   }
   // Returns true if the value of the first item is larger than the value of
   // the second item
   static bool compare_values_descending(const NGramItem &a,
         const NGramItem &b) {
      return a.value > b.value;
   }

   // Numeric
   static bool close_to_0(NGramItem item);
};

class NGram {
public:
   // Constructors, destructor
   NGram();
   NGram(const NGram &src);
   NGram(const NGramConf &conf);
   NGram(const NGramConf &conf, const char *s, unsigned long l);
   virtual ~NGram() {
   } // Nothing to be done here

   // Deep copy
   NGram clone() const;

   // Overloaded operators
   NGram &operator +=(const NGram &arg);
   NGram &operator -=(const NGram &arg);
   NGram &operator *=(double arg);
   double operator *(const NGram &arg) const;
   bool operator ==(const NGram &arg) const;

   // Norms
   double L1norm() const;
   double L2norm() const;
   void reembed(EmbeddingType e) {
      NGram::embedding_fns[e](*this);
   }

   // Bound the values to min <= value <= max
   void trim(double min, double max);
   // Returns the configuration object
   NGramConf getConfiguration() const {
      return *conf;
   }
   // Returns the number of features
   unsigned int getCount() const {
      return count;
   }
   // Returns top N features, sorted by value, descending
   std::vector<NGramItem> getTopN(unsigned int n) const;
};

enum KernelType {
   KERNEL_LINEAR, KERNEL_RBF
};

class Kernel {
public:
   // Virtual destructor
   virtual ~Kernel() {
   }
   // Virtual constructor idiom
   virtual Kernel *clone() const = 0;

   // Virtual method, returns a constant describing the type of this kernel
   virtual KernelType getType() const = 0;
   // Virtual method, computes the distance between two vectors
   virtual double compute(const NGram &a, const NGram &b) const = 0;
   // Loads a new kernel from the input stream
   static Kernel *deserialize(std::istream &in);
};

class LinearKernel: public Kernel {
public:
   // Default constructor
   LinearKernel() {}
   // Constructor for deserialization
   explicit LinearKernel(std::istream &in) {}
   // Virtual destructor
   virtual ~LinearKernel() {
   }
   // Virtual constructor idiom
   static LinearKernel *create(){
      return new LinearKernel();
   }
   virtual LinearKernel *clone() const {
      return new LinearKernel(*this);
   }

   // Virtual method, returns a constant describing the type of this kernel
   virtual KernelType getType() const {
      return KERNEL_LINEAR;
   }
   // Computes the dot product between two vectors
   double compute(const NGram &a, const NGram &b) const {
      return a * b;
   }
};

class RBFKernel: public Kernel {
public:
   // Constructor for deserialization
   explicit RBFKernel(std::istream &in);
   // Constructor
   explicit RBFKernel(double gamma) :
      gamma(gamma) {

   }
   // Virtual constructor idiom
   static RBFKernel *create(){
      return new RBFKernel(10.0);
   }
   virtual RBFKernel *clone() const {
      return new RBFKernel(*this);
   }

   // Virtual method, returns a constant describing the type of this kernel
   virtual KernelType getType() const {
      return KERNEL_RBF;
   }
   // Virtual destructor
   virtual ~RBFKernel() {
   }
   // Computes the RBF distance between two vectors
   double compute(const NGram &a, const NGram &b) const;
};

}

%include "carrays.i"
%array_class(stem::NGram, NGramArray);