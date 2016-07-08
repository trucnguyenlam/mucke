static unsigned long primes[] =
  {
             2,
             3,
             7,
            13,
            31,
            61,
           127,
           251,
           509,
          1021,
          2039,
          4093,
          8191,
         16381,
         32749,
         65521,
        131071,
        262139,
        524287,
       1048573,
       2097143,
       4194301,
       8388593,
      16777213,
      33554393,
      67108859,
     134217689,
     268435399,
     536870909,
    1073741789,
    2147483647,
    (unsigned long)~0 /* just in case */
  };

unsigned long nextPrime(const unsigned long& n)
{
  for(unsigned int i = 0; i < sizeof(primes)/sizeof(unsigned long); i++)
    if(primes[i]>=n)
      return primes[i];
  return 2;
}

unsigned long prevPrime(const unsigned long& n)
{
  for(int i = sizeof(primes)/sizeof(unsigned long) - 1; i>=0; i--)
    if(primes[i]<=n)
      return primes[i];
  return 2;
    
}
