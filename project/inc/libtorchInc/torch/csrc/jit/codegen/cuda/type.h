#pragma once

#include <c10/core/ScalarType.h>
#include <c10/util/Exception.h>
#include <c10/util/Optional.h>

#include <c10/macros/Export.h>

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

namespace torch {
namespace jit {
namespace fuser {
namespace cuda {

// https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
struct TypeHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

// Order of strength
enum class ValType {
  TensorDomain,
  IterDomain,
  TensorView,
  Scalar,
  NamedScalar,
  Predicate,
  TensorIndex,
};

// Manual - The user provides the Bool value. Predicate generation is bypassed.
// Inline corresponds with PredicateCompute::getInlinePredicate
// Unswitch corresponds with UnswitchPredicate::get
// Misaligned - PredicateCompute::getInlinePredicate + Misaligned flag
// Shift - ShiftPredicateInserter::getShiftPredicate
// Padding - ShiftPredicateInserter::getPaddingPredicate
// ReductionWrite - Same as Inline but without reduction axes
enum class PredicateType {
  Manual,
  Inline,
  Unswitch,
  Vectorize,
  Misaligned,
  Shift,
  Padding,
  ReductionWrite
};

// Index type is a convenience type that may be a 64 or 32 signed integer.
// This is helpful for math on indexing/size when we don't know what the index
// type might be. This allows us to prevent assuming the welford count must be
// int64_t which is relatively heavy to carry around. Index will be resolved
// at compile time with KernelIndexMode.
enum class DataType {
  Double,
  Float,
  Half,
  Int,
  Index,
  Int32,
  Bool,
  BFloat16,
  ComplexFloat,
  ComplexDouble,
  // Vectorized types, used for reinterpret casting views
  // TODO: add more vectorized types
  Double_2,
  Float_2,
  // Null
  Null
};

enum class KernelIndexMode { INT32, INT64 };

DataType indexModeToDtype(KernelIndexMode index_mode);

// Returns if the datatype is a floating point type
bool isFloatingPointType(DataType dtype);
// Returns if the datatype is an boolean type
bool isIntegralType(DataType dtype);
// Returns if the datatype is an integer type
bool isBooleanType(DataType dtype);
// Returns if the datatype is a complex type
bool isComplexType(DataType dtype);
// Returns if the datatype is a vector type
bool isVectorType(DataType dtype);
// Return the corresponding vector type
DataType getVectorType(DataType dtype, size_t vec_size);
// Return the vector size for the given vector type
int getVectorSizeFromType(DataType dtype);
// Return the corresponding type of a vector type
DataType getTypeFromVectorType(DataType dtype);
// Return the corresponding scalar of a complex type
DataType getTypeFromComplexType(DataType dtype);

enum class ExprType {
  Invalid,
  UnaryOp,
  BinaryOp,
  TernaryOp,
  ReductionOp,
  GroupedReductionOp,
  BroadcastOp,
  WelfordOp,
  MmaOp,
  TransposeOp,
  ShiftOp,
  GatherOp,
  ViewOp,
  Split,
  ViewAsScalar,
  Merge,
  Allocate,
  BlockSync,
  GridSync,
  InitMagicZero,
  UpdateMagicZero,
  ForLoop,
  IfThenElse,
  GridReduction,
  GroupedGridReduction,
  GridBroadcast,
  GridWelford,
  AllocateFusedReduction
};

enum class UnaryOpType {
  Abs,
  Acos,
  Address,
  Asin,
  Atan,
  Atanh,
  Cast,
  Ceil,
  Cos,
  Cosh,
  Exp,
  Expm1,
  Erf,
  Erfc,
  Floor,
  Frac,
  Gelu,
  Silu,
  Lgamma,
  Log,
  Log10,
  Log1p,
  Log2,
  BitCast,
  Neg,
  RandLike,
  Reciprocal,
  Relu,
  Rsqrt,
  Round,
  Set,
  Sigmoid,
  Sin,
  Sinh,
  Sqrt,
  Tan,
  Tanh,
  Trunc,

  // Might be a bitwise operator or boolean operator.
  Not,

  // Operators returning boolean values
  IsFinite,
  IsInf,
  IsNan,
  IsNegInf,
  IsPosInf,
  IsReal,
};

// Primarily for Not, which could be Not a boolean, or a bitwise not.
bool alsoBooleanOperator(const UnaryOpType uopt);

// TODO: Order of this list is important as it affects type promotion. it's not
// in the right order now.
enum class BinaryOpType {
  // Math Ops
  Add,
  Atan2,
  Div,
  Fmod,
  Max,
  Min,
  Mul,
  Pow,
  Remainder,
  Sub,
  // TypeAs,

  // Integer output ops. If changing modify isIntegerOp
  Mod,
  CeilDiv,
  Lshift,
  Rshift,

  // Logical Ops
  // Int operations, leave position of Mod as first logical op see
  // isLogicalOp(BinaryOpType bopt)
  Eq,
  GE,
  GT,
  LE,
  LT,
  NE,

  // Maybe bitwise or boolean op, leave position of and as first bool/int
  // op. These are ops that have different operators based on output type. See
  // is boolean op. These ops also don't work on floating point inputs.
  And,
  Or,
  Xor
};

// Return if output of operator should be a boolean
bool isIntegerOp(const BinaryOpType bopt);

// Return if output of operator should be a boolean
bool isLogicalOp(const BinaryOpType bopt);

// Operations that could be a bitwise operation or a boolean operation depending
// on input, for example bitwise_and is also used for boolean and in the jit
bool alsoBooleanOperator(const BinaryOpType bopt);

enum class TernaryOpType { Clamp, Lerp, Threshold, Where };

enum class ParallelType {
  BIDz,
  BIDy,
  BIDx,
  TIDz,
  TIDy,
  TIDx,
  Vectorize,
  MisalignedVectorize,
  Unroll,
  Unswitch,
  Mma,
  Serial
};

static constexpr std::array<ParallelType, 6> kParallelTypeThreads = {
    ParallelType::BIDx,
    ParallelType::BIDy,
    ParallelType::BIDz,
    ParallelType::TIDx,
    ParallelType::TIDy,
    ParallelType::TIDz};

static constexpr std::array<ParallelType, 3> kParallelTypeBIDs = {
    ParallelType::BIDx,
    ParallelType::BIDy,
    ParallelType::BIDz};

static constexpr std::array<ParallelType, 3> kParallelTypeTIDs = {
    ParallelType::TIDx,
    ParallelType::TIDy,
    ParallelType::TIDz};

enum class MemoryType { Local, Shared, Global };

// sometimes broadcasted tensors may be inputed in the kernel with an explicit 1
// size. If that size is there, we need to account that there's also a stride
// there, even if the stride = 0. If we don't account for that stride when
// accessing a tensor like: [b2{1}, i0, i1] we would linearize the access like:
// [i0*stride[0] + i1*stride[1]] when it should be: [i0*stride[1] +
// i1*stride[2]]. Broadcasts that translate to a physical memory dim we consider
// "with stride", Broadcasts only through our broadcast op we consider "without
// stride"
enum class IterType {
  Iteration,
  Reduction,
  BroadcastWithStride,
  BroadcastWithoutStride,
  Gather,
  Stride,
  VectorComponent
};

enum class SwizzleType { NoSwizzle, Transpose };

// Used for Iteration Domain mapping modes in ComputeAtMap
enum class IdMappingMode { PERMISSIVE, EXACT, LOOP };

static constexpr std::array<IdMappingMode, 3> kIdMappingModes = {
    IdMappingMode::PERMISSIVE,
    IdMappingMode::EXACT,
    IdMappingMode::LOOP};

// Returns if function needs an f suffix on the operator when operating on a
// float value i.e. sin->sinf
bool needFloatSuffix(UnaryOpType t);
bool needFloatSuffix(BinaryOpType t);

ValType promote_type(const ValType& t1, const ValType& t2);
DataType promote_type(const DataType& t1, const DataType& t2);

// If type cannot be found (i.e. codegen does not support provided type) returns
// DataType::Null
TORCH_CUDA_CU_API DataType aten_to_data_type(const at::ScalarType& scalar_type);
TORCH_CUDA_CU_API at::ScalarType data_type_to_aten(const DataType& data_type);

TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const ValType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const PredicateType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const DataType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const ExprType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const UnaryOpType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const BinaryOpType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const TernaryOpType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const ParallelType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const MemoryType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const IterType);
TORCH_CUDA_CU_API std::ostream& operator<<(std::ostream&, const IdMappingMode);

std::string stringifyBooleanOp(const UnaryOpType);
std::string stringifyBooleanOp(const BinaryOpType);

std::string stringifyThreadSize(const ParallelType);
std::string stringifyThread(const ParallelType);
std::string typePrefix(const DataType);

// TODO: ThreadDim should be BlockDim and BlockDim should be GridDim
// Returns if parallel type is TID[x, y, z]
TORCH_CUDA_CU_API bool isParallelTypeThreadDim(ParallelType);
// Returns if parallel type is BID[x, y, z]
TORCH_CUDA_CU_API bool isParallelTypeBlockDim(ParallelType);
// Returns if parallel type is a grid or block parallelization dimension
TORCH_CUDA_CU_API bool isParallelTypeThread(ParallelType);

TORCH_CUDA_CU_API bool isParallelTypeVectorize(ParallelType);

TORCH_CUDA_CU_API c10::optional<std::string> inline_op_str(const UnaryOpType);
TORCH_CUDA_CU_API c10::optional<std::string> inline_op_str(const BinaryOpType);
TORCH_CUDA_CU_API c10::optional<std::string> integer_op_str(const BinaryOpType);
TORCH_CUDA_CU_API c10::optional<std::string> bool_op_str(const BinaryOpType);

TORCH_CUDA_CU_API c10::optional<std::string> cast_func_str(
    const std::pair<DataType, DataType>&);

TORCH_CUDA_CU_API size_t dataTypeSize(DataType type);

// If the index type is known it will be automatically used here
TORCH_CUDA_CU_API size_t dataTypeSize(DataType type, DataType index_type);

enum class LaunchConfigType {
  Compatible,
  SharedMemory,
  BIDz,
  BIDy,
  BIDx,
  TIDz,
  TIDy,
  TIDx
};

const char* const kMagicZeroName = "nvfuser_zero";

} // namespace cuda
} // namespace fuser
} // namespace jit
} // namespace torch
