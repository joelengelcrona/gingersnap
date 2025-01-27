#include "emu_generic.h"
#include "mips64msb/mips64msb.h"
#include "riscv/riscv.h"

#include "../utils/logger.h"

void
emu_load_elf(emu_t* self, const target_t* target)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->load_elf(self->riscv, target);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->load_elf(self->mips64msb, target);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

void
emu_build_stack(emu_t* self, const target_t* target)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->build_stack(self->riscv, target);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->build_stack(self->mips64msb, target);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

void
emu_execute(emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->execute(self->riscv);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->execute(self->mips64msb);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

emu_t*
emu_fork(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
        {
            emu_t* forked = calloc(1, sizeof(emu_t));
            forked->riscv = self->riscv->fork(self->riscv);
            return forked;
        }
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
        {
            emu_t* forked = calloc(1, sizeof(emu_t));
            forked->mips64msb = self->mips64msb->fork(self->mips64msb);
            return forked;
        }
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

void
emu_reset(emu_t* self, const emu_t* src)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->reset(self->riscv, src->riscv);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->reset(self->mips64msb, src->mips64msb);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

void
emu_print_regs(emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->print_regs(self->riscv);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->print_regs(self->mips64msb);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

enum_emu_exit_reasons_t
emu_run(emu_t* self, emu_stats_t* stats)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->run(self->riscv, stats);
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->run(self->mips64msb, stats);
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

enum_emu_exit_reasons_t
emu_run_until(emu_t* self, emu_stats_t* stats, const uint64_t break_adr)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->run_until(self->riscv, stats, break_adr);
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->run_until(self->mips64msb, stats, break_adr);
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

void
emu_stack_push(emu_t* self, uint8_t bytes[], size_t nb_bytes)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            self->riscv->stack_push(self->riscv, bytes, nb_bytes);
            break;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            self->mips64msb->stack_push(self->mips64msb, bytes, nb_bytes);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

enum_supported_archs_t
emu_get_arch(const emu_t* self)
{
    return self->arch;
}

uint64_t
emu_get_pc(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->get_pc(self->riscv);
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->get_pc(self->mips64msb);
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

uint64_t
emu_get_stack_size(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->stack_size;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->stack_size;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

mmu_t*
emu_get_mmu(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->mmu;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->mmu;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

enum_emu_exit_reasons_t
emu_get_exit_reason(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->exit_reason;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->exit_reason;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

bool
emu_get_new_coverage(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->new_coverage;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->new_coverage;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

corpus_t*
emu_get_corpus(const emu_t* self)
{
    switch (self->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
            return self->riscv->corpus;
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            return self->mips64msb->corpus;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}

emu_t*
emu_create(enum_supported_archs_t arch, size_t memory_size, corpus_t* corpus)
{
    emu_t* emu = calloc(1, sizeof(emu_t));

    switch (arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
        {
            emu->riscv = riscv_create(memory_size, corpus);
            break;
        }
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            emu->mips64msb = mips64msb_create(memory_size, corpus);
            break;
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }

    emu->arch             = arch;
    emu->load_elf         = emu_load_elf;
    emu->build_stack      = emu_build_stack;
    emu->execute          = emu_execute;
    emu->fork             = emu_fork;
    emu->reset            = emu_reset;
    emu->print_regs       = emu_print_regs;
    emu->run              = emu_run;
    emu->run_until        = emu_run_until;
    emu->stack_push       = emu_stack_push;
    emu->get_arch         = emu_get_arch;
    emu->get_pc           = emu_get_pc;
    emu->get_stack_size   = emu_get_stack_size;
    emu->get_mmu          = emu_get_mmu;
    emu->get_exit_reason  = emu_get_exit_reason;
    emu->get_new_coverage = emu_get_new_coverage;
    emu->get_corpus       = emu_get_corpus;

    return emu;
}

void
emu_destroy(emu_t* emu)
{
    switch (emu->arch)
    {
        case ENUM_SUPPORTED_ARCHS_RISCV64I_LSB:
        {
            riscv_destroy(emu->riscv);
            break;
        }
        case ENUM_SUPPORTED_ARCHS_MIPS64_MSB:
            ginger_log(ERROR, "MIPS not yet implemented!\n");
            abort();
        default:
            ginger_log(ERROR, "Unrecognized arch!\n");
            abort();
    }
}
