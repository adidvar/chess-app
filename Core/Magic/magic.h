#ifndef MAGIC_H
#define MAGIC_H
#include <inttypes.h>

class Magic
{
    uint64_t mask_;
    uint64_t plot_mask_;
    uint64_t magic_;
    uint8_t shift_;
public:
    Magic(uint64_t mask);
    Magic(uint64_t mask, uint64_t magic, uint64_t shift);
    /// From plot view to normal
    uint64_t Encode(uint64_t value) const;
    /// From normal to plot view
    uint64_t Decode(uint64_t value) const;

    uint64_t Mask() const{return mask_;};
    uint64_t MagicNum() const{return magic_;};
    uint64_t Shift() const{return shift_;};
    uint64_t PlotMask() const{return plot_mask_;};
};

#endif
