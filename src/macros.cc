int ld_ceiling_table[257] =
{
  0, 0,						// 0,1
  1,						// 2
  2, 2,						// 3 4
  3, 3, 3, 3,					// 5..8
  4, 4, 4, 4, 4, 4, 4, 4,			// 9..16
  5, 5, 5, 5, 5, 5, 5, 5,			// 17..32
  5, 5, 5, 5, 5, 5, 5, 5,			// 17..32
  6, 6, 6, 6, 6, 6, 6, 6,			// 33..64
  6, 6, 6, 6, 6, 6, 6, 6,			// 33..64
  6, 6, 6, 6, 6, 6, 6, 6,			// 33..64
  6, 6, 6, 6, 6, 6, 6, 6,			// 33..64
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..127
  7, 7, 7, 7, 7, 7, 7, 7,			// 65..128
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8,			// 129..256
  8, 8, 8, 8, 8, 8, 8, 8			// 129..256
};

int abs(int i) { return i<0 ? -i : i; }
