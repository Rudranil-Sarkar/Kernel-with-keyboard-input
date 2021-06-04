#ifndef __gdt_H__
#define __gdt_H__

#include "types.h"

class GlobalDescriptorTable {

	public:
		GlobalDescriptorTable();
		~GlobalDescriptorTable();
		uint16_t CodeSegmentChooser();
		uint16_t DataSegmentChooser();

	public:
		class SegmentDescriptor {
		
			public:
				uint16_t limit_lo;
				uint16_t base_lo;
				uint8_t base_hi;
				uint8_t type;
				uint8_t flag_limit_hi;
				uint8_t base_vhi;

			public:
				SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags);
				uint32_t Base();
				uint32_t Limit();
		
		} __attribute__((packed));

		SegmentDescriptor nullSegmentSelector;
		SegmentDescriptor unusedSegmentSelector;
		SegmentDescriptor codeSegmentSelector;
		SegmentDescriptor dataSegmentSelector;

};

#endif
