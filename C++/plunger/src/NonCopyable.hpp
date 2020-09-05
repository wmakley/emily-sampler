#ifndef NON_COPYABLE_HPP
#define NON_COPYABLE_HPP

class NonCopyable
{
protected:
  NonCopyable() {}
  ~NonCopyable() {}

private:
  NonCopyable(const NonCopyable &);
  NonCopyable &operator=(const NonCopyable &);
};

#endif
