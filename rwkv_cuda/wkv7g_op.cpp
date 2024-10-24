#include <torch/extension.h>

#include <c10/core/DispatchKey.h>
#include <torch/library.h>

typedef at::BFloat16 bf16;

void cuda_forward(int B, int T, int C, int H, bf16 *r, bf16 *w, bf16 *k, bf16 *v, bf16 *a, bf16 *b, bf16 *y, float *saa, float* sss);
void cuda_backward(int B, int T, int C, int H, bf16 *r, bf16 *w, bf16 *k, bf16 *v, bf16 *a, bf16 *b, float *saa, float* sss, float* zzz, bf16 *gy, bf16 *gr, bf16 *gw, bf16 *gk, bf16 *gv, bf16 *ga, bf16 *gb);

void forward(int64_t B, int64_t T, int64_t C, int64_t H, torch::Tensor &r, torch::Tensor &w, torch::Tensor &k, torch::Tensor &v, torch::Tensor &a, torch::Tensor &b, torch::Tensor &y, torch::Tensor &saa, torch::Tensor &sss) {
    if (r.is_meta()) {
        return;
    }
    cuda_forward(B, T, C, H, r.data_ptr<bf16>(), w.data_ptr<bf16>(), k.data_ptr<bf16>(), v.data_ptr<bf16>(), a.data_ptr<bf16>(), b.data_ptr<bf16>(), y.data_ptr<bf16>(), saa.data_ptr<float>(), sss.data_ptr<float>());
}
void backward(int64_t B, int64_t T, int64_t C, int64_t H, torch::Tensor &r, torch::Tensor &w, torch::Tensor &k, torch::Tensor &v, torch::Tensor &a, torch::Tensor &b, torch::Tensor &saa, torch::Tensor &sss, torch::Tensor &zzz, torch::Tensor &gy, torch::Tensor &gr, torch::Tensor &gw, torch::Tensor &gk, torch::Tensor &gv, torch::Tensor &ga, torch::Tensor &gb) {
    if (r.is_meta()) {
        return;
    }
    cuda_backward(B, T, C, H, r.data_ptr<bf16>(), w.data_ptr<bf16>(), k.data_ptr<bf16>(), v.data_ptr<bf16>(), a.data_ptr<bf16>(), b.data_ptr<bf16>(), saa.data_ptr<float>(), sss.data_ptr<float>(), zzz.data_ptr<float>(), gy.data_ptr<bf16>(), gr.data_ptr<bf16>(), gw.data_ptr<bf16>(), gk.data_ptr<bf16>(), gv.data_ptr<bf16>(), ga.data_ptr<bf16>(), gb.data_ptr<bf16>());
}

// TORCH_LIBRARY(wkv7g, m) {
//     m.def("forward", forward);
//     m.def("backward", backward);
// }

TORCH_LIBRARY_FRAGMENT(wkv7g, m) {
    m.def(TORCH_SELECTIVE_SCHEMA(
        "wkv7g::forward(int B, int T, int C, int H, Tensor r, Tensor w, Tensor k, Tensor v, Tensor a, Tensor b, Tensor y, Tensor saa, Tensor sss) -> ()"));
    m.def(TORCH_SELECTIVE_SCHEMA(
        "wkv7g::backward(int B, int T, int C, int H, Tensor r, Tensor w, Tensor k, Tensor v, Tensor a, Tensor b, Tensor saa, Tensor sss, Tensor zzz, Tensor gy, Tensor gr, Tensor gw, Tensor gk, Tensor gv, Tensor ga, Tensor gb) -> ()"));
}

#define TORCH_LIBRARY_IMPL_WITH_META(ns, k, m, block)                   \
  TORCH_LIBRARY_IMPL(ns, k, m){block} TORCH_LIBRARY_IMPL(ns, Meta, m) { \
    block                                                               \
  }

TORCH_LIBRARY_IMPL_WITH_META(wkv7g, DefaultBackend, m, {
  m.impl(TORCH_SELECTIVE_NAME("wkv7g::forward"), TORCH_FN(forward));
  m.impl(TORCH_SELECTIVE_NAME("wkv7g::backward"), TORCH_FN(backward));
})

#undef TORCH_LIBRARY_IMPL_WITH_META
