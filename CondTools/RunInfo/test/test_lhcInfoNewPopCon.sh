#!/bin/sh

LOCAL_TEST_DIR=src/CondTools/RunInfo/test
SCRIPTS_DIR=src/CondTools/RunInfo/python

function die { echo Failure $1: status $2 ; exit $2 ; }

assert_equal() {
  expected="$1"
  actual="$2"
  message="$3"
  
  if [ "$expected" != "$actual" ]; then
    echo "$message: Expected $expected, but got $actual"
    exit 1
  fi
}

function assert_found_fills {
  log_file="$1"
  shift
  for fill_nr in "$@"; do
    if ! grep -q "Found fill $fill_nr" "$log_file"; then
      echo "Error: LHCInfoPerFillPopConAnalyzerDuringFill didn't find fill $fill_nr"
      exit 1
    fi
  done
}

rm -f lhcinfo_pop_unit_test.db

echo "testing LHCInfoPerFillPopConAnalyzerEndFill for startTime=\"2022-10-24 01:00:00.000\" endTime=\"2022-10-24 20:00:00.000\"" 
cmsRun ${SCRIPTS_DIR}/LHCInfoPerFillPopConAnalyzerEndFill.py \
    destinationConnection="sqlite_file:lhcinfo_pop_unit_test.db" \
    startTime="2022-10-24 01:00:00.000" endTime="2022-10-24 20:00:00.000" \
    tag=fill_end_test > fill_end_test.log || die "cmsRun LHCInfoPerFillWriter_cfg.py" $?
assert_equal 7 `cat fill_end_test.log | grep -E '^Since ' | \
    wc -l` "LHCInfoPerFillPopConAnalyzerEndFill written wrong number of payloads"
assert_found_fills ls_during_test.log 8307 8309

echo "testing LHCInfoPerLSPopConAnalyzerEndFill for startTime=\"2022-10-24 01:00:00.000\" endTime=\"2022-10-24 20:00:00.000\"" 
cmsRun ${SCRIPTS_DIR}/LHCInfoPerLSPopConAnalyzerEndFill.py \
    destinationConnection="sqlite_file:lhcinfo_pop_unit_test.db" \
    startTime="2022-10-24 01:00:00.000" endTime="2022-10-24 20:00:00.000" \
    tag=ls_end_test > ls_end_test.log || die "cmsRun LHCInfoPerLSWriter_cfg.py" $?
assert_equal 169 `cat ls_end_test.log | grep -E '^Since ' | \
    wc -l` "LHCInfoPerLSPopConAnalyzerEndFill written wrong number of payloads"
assert_found_fills ls_during_test.log 8307 8309

echo "testing LHCInfoPerFillPopConAnalyzerDuringFill for startTime=\"2022-10-24 01:00:00.000\" endTime=\"2022-10-24 20:00:00.000\"" 
cmsRun ${SCRIPTS_DIR}/LHCInfoPerFillPopConAnalyzerDuringFill.py \
    destinationConnection="sqlite_file:lhcinfo_pop_unit_test.db" \
    startTime="2022-10-24 01:00:00.000" endTime="2022-10-24 20:00:00.000" \
    tag=fill_during_test > fill_during_test.log || die "cmsRun LHCInfoPerFillWriter_cfg.py" $?
#TODO Is 5 really correct?
assert_equal 5 `cat fill_during_test.log | grep -E '^Since ' | \
    wc -l` "LHCInfoPerFillPopConAnalyzerDuringFill written wrong number of payloads"
assert_found_fills fill_during_test.log 8307 8309

echo "testing LHCInfoPerLSPopConAnalyzerDuringFill for startTime=\"2022-10-24 01:00:00.000\" endTime=\"2022-10-24 20:00:00.000\"" 
cmsRun ${SCRIPTS_DIR}/LHCInfoPerLSPopConAnalyzerDuringFill.py \
    destinationConnection="sqlite_file:lhcinfo_pop_unit_test.db" \
    startTime="2022-10-24 01:00:00.000" endTime="2022-10-24 20:00:00.000" \
    tag=ls_during_test > ls_during_test.log || die "cmsRun LHCInfoPerLSWriter_cfg.py" $?
assert_equal 1 `cat ls_during_test.log | grep -E '^Since ' | \
    wc -l` "LHCInfoPerLSPopConAnalyzerDuringFill written wrong number of payloads"
assert_found_fills ls_during_test.log 8307 8309