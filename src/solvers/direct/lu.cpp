/* ************************************************************************
 * Copyright 2018 Advanced Micro Devices, Inc.
 *
 * ************************************************************************ */

#include "../../utils/def.hpp"
#include "lu.hpp"

#include "../../base/local_matrix.hpp"
#include "../../base/local_vector.hpp"

#include "../../utils/log.hpp"

#include <math.h>
#include <complex>

namespace rocalution {

template <class OperatorType, class VectorType, typename ValueType>
LU<OperatorType, VectorType, ValueType>::LU() {

  log_debug(this, "LU::LU()");

}

template <class OperatorType, class VectorType, typename ValueType>
LU<OperatorType, VectorType, ValueType>::~LU() {

  log_debug(this, "LU::~LU()");

  this->Clear();

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::Print(void) const {

  LOG_INFO("LU solver");

}


template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::PrintStart_(void) const {

  LOG_INFO("LU direct solver starts");

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::PrintEnd_(void) const {

  LOG_INFO("LU ends");

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::Build(void) {

  log_debug(this, "LU::Build()",
            this->build_,
            " #*# begin");

  if (this->build_ == true)
    this->Clear();

  assert(this->build_ == false);
  this->build_ = true;

  assert(this->op_ != NULL);
  assert(this->op_->GetM() == this->op_->GetN());
  assert(this->op_->GetM() > 0);

  this->lu_.CloneFrom(*this->op_);
  this->lu_.LUFactorize();

  log_debug(this, "LU::Build()",
            this->build_,
            " #*# end");

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::Clear(void) {

  log_debug(this, "LU::Clear()",
            this->build_);

  if (this->build_ == true) {

    this->lu_.Clear();
    this->build_ = false;

  }

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::MoveToHostLocalData_(void) {

  log_debug(this, "LU::MoveToHostLocalData_()",
            this->build_);

  if (this->build_ == true)
    this->lu_.MoveToHost();

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::MoveToAcceleratorLocalData_(void) {

  log_debug(this, "LU::MoveToAcceleratorLocalData_()",
            this->build_);

  if (this->build_ == true)
    this->lu_.MoveToAccelerator();

}

template <class OperatorType, class VectorType, typename ValueType>
void LU<OperatorType, VectorType, ValueType>::Solve_(const VectorType &rhs, VectorType *x) {

  log_debug(this, "LU::Solve_()",
            " #*# begin",
            (const void*&)rhs,
            x);

  assert(x != NULL);
  assert(x != &rhs);
  assert(this->build_ == true);

  this->lu_.LUSolve(rhs, x);

  log_debug(this, "LU::Solve_()",
            " #*# end");

}


template class LU< LocalMatrix<double>, LocalVector<double>, double >;
template class LU< LocalMatrix<float>,  LocalVector<float>, float >;
#ifdef SUPPORT_COMPLEX
template class LU< LocalMatrix<std::complex<double> >,  LocalVector<std::complex<double> >, std::complex<double> >;
template class LU< LocalMatrix<std::complex<float> >,  LocalVector<std::complex<float> >, std::complex<float> >;
#endif

} // namespace rocalution
