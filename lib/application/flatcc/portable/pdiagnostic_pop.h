#ifndef PDIAGNOSTIC_POP_H
#define PDIAGNOSTIC_POP_H

#if defined(PDIAGNOSTIC_PUSHED_MSVC)
#pragma warning( pop )
#undef PDIAGNOSTIC_PUSHED_MSVC
#elif defined(PDIAGNOSTIC_PUSHED_CLANG)
#pragma clang diagnostic pop
#undef PDIAGNOSTIC_PUSHED_CLANG
#elif defined(PDIAGNOSTIC_PUSHED_GCC)
#pragma GCC diagnostic pop
#undef PDIAGNOSTIC_PUSHED_GCC
#endif

#endif /* PDIAGNOSTIC_POP_H */
