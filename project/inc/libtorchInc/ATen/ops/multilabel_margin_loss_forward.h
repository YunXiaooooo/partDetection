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



#include <ATen/ops/multilabel_margin_loss_forward_ops.h>

namespace at {


// aten::multilabel_margin_loss_forward.output(Tensor self, Tensor target, int reduction, *, Tensor(a!) output, Tensor(b!) is_target) -> (Tensor(a!), Tensor(b!))
TORCH_API inline ::std::tuple<at::Tensor &,at::Tensor &> multilabel_margin_loss_forward_out(at::Tensor & output, at::Tensor & is_target, const at::Tensor & self, const at::Tensor & target, int64_t reduction) {
    return at::_ops::multilabel_margin_loss_forward_output::call(self, target, reduction, output, is_target);
}

// aten::multilabel_margin_loss_forward.output(Tensor self, Tensor target, int reduction, *, Tensor(a!) output, Tensor(b!) is_target) -> (Tensor(a!), Tensor(b!))
TORCH_API inline ::std::tuple<at::Tensor &,at::Tensor &> multilabel_margin_loss_forward_outf(const at::Tensor & self, const at::Tensor & target, int64_t reduction, at::Tensor & output, at::Tensor & is_target) {
    return at::_ops::multilabel_margin_loss_forward_output::call(self, target, reduction, output, is_target);
}

// aten::multilabel_margin_loss_forward(Tensor self, Tensor target, int reduction) -> (Tensor output, Tensor is_target)
TORCH_API inline ::std::tuple<at::Tensor,at::Tensor> multilabel_margin_loss_forward(const at::Tensor & self, const at::Tensor & target, int64_t reduction) {
    return at::_ops::multilabel_margin_loss_forward::call(self, target, reduction);
}

}