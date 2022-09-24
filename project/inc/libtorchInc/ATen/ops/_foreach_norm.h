#pragma once

// @generated by torchgen/gen.py from Function.h

#include <ATen/Context.h>
#include <ATen/DeviceGuard.h>
#include <ATen/TensorUtils.h>
#include <ATen/TracerMode.h>
#include <ATen/core/Generator.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>



#include <ATen/ops/_foreach_norm_ops.h>

namespace at {


// aten::_foreach_norm.Scalar(Tensor[] tensors, Scalar ord=2) -> Tensor[]
TORCH_API inline ::std::vector<at::Tensor> _foreach_norm(at::TensorList tensors, const at::Scalar & ord=2) {
    return at::_ops::_foreach_norm_Scalar::call(tensors, ord);
}

}