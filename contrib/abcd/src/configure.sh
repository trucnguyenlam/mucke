#!/bin/sh

##############################################################################

AUTOMATIC=true

while [ $# -ne 0 ]
do
  case $1 in
    --manual) AUTOMATIC=false;;
    *) echo "*** $0: unknown command line option $1" 1>&2; exit 1;;
  esac
  shift
done

##############################################################################
# find a working echo -n

if test "x`echo -n`" = x-n
then
  if `printf test 2>&1 > /dev/null`
  then
    PRINTF=printf
  else
    PRINTF=echo
  fi
else
  PRINTF="echo -n"
fi 

echo
echo "############# BEGIN CONFIGURATION #################"
echo

if test -f ./config.cache
then
  $PRINTF "reading config.cache ..."
  . ./config.cache
  echo " done."
  found_cache=yes
else
  echo "no config.cache found."
  found_cache=no

  . ./configure.init
fi

echo

###########################################################
while true
do
#                                    ?
  $PRINTF "define DEBUG              ? [$define_debug] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break
  else
    read answer
    case "X$answer" in
      X)	break;;
      Xyes)	define_debug=yes; break;;
      Xno)	define_debug=no; break;;
      X*)	echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

if test "$define_debug" = yes -a "$found_cache" = no
then
  generate=functions
  define_statistics=yes
fi

###########################################################
while true
do
#                                    ?
  $PRINTF "define STATISTICS         ? [$define_statistics] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)	break;;
      Xyes)	define_statistics=yes; break;;
      Xno)	define_statistics=no; break;;
      X*)	echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
while true
do
#                                    ?
  $PRINTF "define DISTRIBUTION_STATS ? [$define_distribution_stats] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	define_distribution_stats=yes; break;;
      Xno)	define_distribution_stats=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
while true
do
#                                    ?
  $PRINTF "define CALL_STATS         ? [$define_call_stats] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	define_call_stats=yes; break;;
      Xno)	define_call_stats=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
while true
do
#                                    ?
  $PRINTF "define PRIORITY           ? [$define_priority] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	define_priority=yes; break;;
      Xno)	define_priority=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
while true
do
#                                    ?
  $PRINTF "manual bit stuffing       ? [$manual_bit_stuffing] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	manual_bit_stuffing=yes; break;;
      Xno)	manual_bit_stuffing=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
if test "$manual_bit_stuffing" = no
then
  while true
  do
  #                                  ?
    $PRINTF "split left successor    ? [$split_left_successor] "
    if [ $AUTOMATIC = true ]
    then
      echo ""
      break;
    else
      read answer
      case "X$answer" in
	X)		break;;
	Xyes)	split_left_successor=yes; break;;
	Xno)	split_left_successor=no; break;;
	X*)		echo "*** 'yes' or 'no' expected";;
      esac
      fi
    done
fi

###########################################################
while true
do
#                                    ?
  $PRINTF "use double's              ? [$use_doubles] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	use_doubles=yes; break;;
      Xno)	use_doubles=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done


###########################################################
while true
do
#                                    ?
  $PRINTF "use shared bit            ? [$use_shared] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	use_shared=yes; break;;
      Xno)	use_shared=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done


###########################################################
while true
do
#                                    ?
  $PRINTF "use power of 2 sizes      ? [$use_power2] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	use_power2=yes; break;;
      Xno)	use_power2=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
while true
do
#                                    ?
  $PRINTF "multiplicative hashing    ? [$multiplicative_hashing] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xyes)	multiplicative_hashing=yes; break;;
      Xno)	multiplicative_hashing=no; break;;
      X*)		echo "*** 'yes' or 'no' expected";;
    esac
  fi
done

###########################################################
_generate="$generate"
while true
do
#                                    ?
  $PRINTF "macros/functions/custom   ? [$generate] "
  if [ $AUTOMATIC = true ]
  then
    echo ""
    break;
  else
    read answer
    case "X$answer" in
      X)		break;;
      Xmacros)	generate="macros"; break;;
      Xfunctions)	generate="functions"; break;;
      Xcustom)	generate="custom"; break;;
      X*)		echo "*** '$answer' not supported";;
    esac
  fi
done

if test "$generate" = custom
then
  if test ! -f no-macros
  then
    echo "*** could not find the file 'no-macros'" 2>&1
    echo "*** the code for the functions listed in that file" 2>&1
    echo "*** is not generated as macro" 2>&1
    exit 1
  fi
  functions=`cat no-macros`
  echo
  for f in $functions
  do
    echo "${f}=function"
    assignments="$assignments ${f}=function"
  done
  echo
fi

echo

###########################################################
$PRINTF "saving config.cache and generating config.h ..."

. ./configure.write

echo " done."

echo
echo "############# END CONFIGURATION ###################"
echo
