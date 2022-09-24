#pragma once

// @generated by torchgen/gen.py from NativeFunction.h

#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>
#include <c10/core/QScheme.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <tuple>
#include <vector>
#include <ATen/ops/topk_meta.h>

namespace at {
namespace native {

struct TORCH_API structured_topk_out_cpu : public at::meta::structured_topk {
void impl(const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted, const at::Tensor & values, const at::Tensor & indices);
};
struct TORCH_API structured_topk_out_cuda : public at::meta::structured_topk {
void impl(const at::Tensor & self, int64_t k, int64_t dim, bool largest, bool sorted, const at::Tensor & values, const at::Tensor & indices);
};
TORCH_API ::std::tuple<at::Tensor,at::Tensor> topk_quantized_cpu(const at::Tensor & self, int64_t k, int64_t dim=-1, bool largest=true, bool sorted=true);

} // namespace native
} // namespace at