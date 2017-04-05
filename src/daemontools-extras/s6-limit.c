/* ISC license. */

#include <sys/types.h>
#include <sys/resource.h>
#include <skalibs/strerr2.h>
#include <skalibs/sgetopt.h>
#include <skalibs/uint64.h>
#include <skalibs/djbunix.h>

static const char USAGE[] = 
  "s6-limit OPTIONS prog...\n"
  "  SOFT  HARD  RESOURCE\n"
  "  -a    -A    allbytes\n"
  "  -c    -C    corebytes\n"
  "  -d    -D    databytes\n"
  "  -f    -F    filebytes\n"
  "  -l    -L    lockbytes\n"
  "  -m    -M    membytes\n"
  "  -o    -O    openfiles\n"
  "  -p    -P    processes\n"
  "  -r    -R    residentbytes\n"
  "  -s    -S    stackbytes\n"
  "  -t    -T    cpusecs";

static void doit (int res, int opt, char const *arg)
{
  struct rlimit r ;
  if (getrlimit(res, &r) < 0) strerr_diefu1sys(111, "getrlimit") ;
  if (opt >= 'A' && opt <= 'Z')
  {
    uint64_t n ;
    if (!uint640_scan(arg, &n)) strerr_dieusage(100, USAGE) ;
    r.rlim_max = (rlim_t)n ;
  }
  else
  {
    if ((arg[0] == '=') && !arg[1]) r.rlim_cur = r.rlim_max ;
    else
    {
      uint64_t n ;
      if (!uint640_scan(arg, &n)) strerr_dieusage(100, USAGE) ;
      if (n > (uint64_t)r.rlim_max) n = (uint64_t)r.rlim_max ;
      r.rlim_cur = (rlim_t)n ;
    }
  }
  if (setrlimit(res, &r) < 0) strerr_diefu1sys(111, "setrlimit") ;
}

int main (int argc, char const *const *argv, char const *const *envp)
{
  subgetopt_t l = SUBGETOPT_ZERO ;
  PROG = "s6-softlimit" ;
  for (;;)
  {
    int opt = subgetopt_r(argc, argv, "a:c:d:f:l:m:o:p:r:s:t:A:C:D:F:L:M:O:P:R:S:T:", &l) ;
    if (opt == -1) break ;
    switch (opt)
    {
      case 'a' :
      case 'A' :
#ifdef RLIMIT_AS
        doit(RLIMIT_AS, opt, l.arg) ;
#endif
#ifdef RLIMIT_VMEM
        doit(RLIMIT_VMEM, opt, l.arg) ;
#endif
        break ;
      case 'c' :
      case 'C' :
#ifdef RLIMIT_CORE
        doit(RLIMIT_CORE, opt, l.arg) ;
#endif
        break ;
      case 'd' :
      case 'D' :
#ifdef RLIMIT_DATA
        doit(RLIMIT_DATA, opt, l.arg) ;
#endif
        break ;
      case 'f' :
      case 'F' :
#ifdef RLIMIT_FSIZE
        doit(RLIMIT_FSIZE, opt, l.arg) ;
#endif
        break ;
      case 'l' :
      case 'L' :
#ifdef RLIMIT_MEMLOCK
        doit(RLIMIT_MEMLOCK, opt, l.arg) ;
#endif
        break ;
      case 'm' :
      case 'M' :
#ifdef RLIMIT_DATA
        doit(RLIMIT_DATA, opt, l.arg) ;
#endif
#ifdef RLIMIT_STACK
        doit(RLIMIT_STACK, opt, l.arg) ;
#endif
#ifdef RLIMIT_MEMLOCK
        doit(RLIMIT_MEMLOCK, opt, l.arg) ;
#endif
#ifdef RLIMIT_VMEM
        doit(RLIMIT_VMEM, opt, l.arg) ;
#endif
#ifdef RLIMIT_AS
        doit(RLIMIT_AS, opt, l.arg) ;
#endif
	break ;
      case 'o' :
      case 'O' :
#ifdef RLIMIT_NOFILE
        doit(RLIMIT_NOFILE, opt, l.arg) ;
#endif
#ifdef RLIMIT_OFILE
        doit(RLIMIT_OFILE, opt, l.arg) ;
#endif
        break ;
      case 'p' :
      case 'P' :
#ifdef RLIMIT_NPROC
        doit(RLIMIT_NPROC, opt, l.arg) ;
#endif
        break ;
      case 'r' :
      case 'R' :
#ifdef RLIMIT_RSS
        doit(RLIMIT_RSS, opt, l.arg) ;
#endif
        break ;
      case 's' :
      case 'S' :
#ifdef RLIMIT_STACK
        doit(RLIMIT_STACK, opt, l.arg) ;
#endif
        break ;
      case 't' :
      case 'T' :
#ifdef RLIMIT_CPU
        doit(RLIMIT_CPU, opt, l.arg) ;
#endif
        break ;
    }
  }
  argc -= l.ind ; argv += l.ind ;
  if (!argc) strerr_dieusage(100, USAGE) ;
  pathexec_run(argv[0], argv, envp) ;
  strerr_dieexec(111, argv[0]) ;
}
