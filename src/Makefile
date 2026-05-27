# Makefile for octave_boost
# Auto-detects mkoctfile and Boost headers.
# No autoconf needed — works out of the box.
#
# Usage:
#   make          — build all .oct files
#   make clean    — remove build artifacts
#   make install  — install into Octave's package directory
#
# Override variables:
#   MKOCTFILE     — path to mkoctfile          (default: auto-detected)
#   BOOST_CPPFLAGS — Boost include path         (default: auto-detected)
#   CXXFLAGS      — extra compiler flags        (default: -Wall -Wextra)
#
# Examples:
#   make BOOST_CPPFLAGS="-I/usr/include/boost"
#   make MKOCTFILE=/usr/bin/mkoctfile CXXFLAGS="-Wall -O2"

# --- Auto-detect mkoctfile ---
# Try several methods to find mkoctfile (cross-platform)
MKOCTFILE ?= $(shell \
  command -v mkoctfile 2>/dev/null || \
  which mkoctfile 2>/dev/null || \
  type mkoctfile 2>/dev/null | awk '{print $$3}' || \
  echo "" )
ifeq ($(MKOCTFILE),)
$(error "mkoctfile not found. Install octave-devel / liboctave-dev or add it to PATH.")
endif

# --- Auto-detect Boost headers ---
# For "#include <boost/...>" to work, BOOST_CPPFLAGS must point to the
# parent directory of the "boost" folder.
#
# Layout 1 (common): ../boost/ contains accumulators/, chrono/, etc.
#   -> BOOST_CPPFLAGS = -I..   (from src/Makefile, parent of boost/)
#   -> resolves: ../boost/accumulators/accumulators.hpp
#
# Layout 2 (nested): ../boost/boost/ contains accumulators/, chrono/, etc.
#   -> BOOST_CPPFLAGS = -I../boost
#   -> resolves: ../boost/boost/accumulators/accumulators.hpp
#
# Auto-detect both layouts.

ifneq ($(wildcard ../boost/boost/version.hpp),)
  # Layout 2: nested boost/boost/ structure
  BOOST_CPPFLAGS ?= -I../boost
else ifneq ($(wildcard ../boost/version.hpp),)
  # Layout 1: boost/ is the include dir itself; parent holds "boost/"
  # From src/, ".." = <project_root>/octave_boost, "../.." = <project_root>
  # and ../boost/ = <project_root>/boost which has the subdirs.
  # So to resolve #include <boost/foo.hpp> we need -I../..
  BOOST_CPPFLAGS ?= -I../..
else ifneq ($(wildcard ../../boost/version.hpp),)
  BOOST_CPPFLAGS ?= -I../..
endif
BOOST_CPPFLAGS ?=

# --- Compiler flags ---
CXXFLAGS ?= -Wall -Wextra -std=c++17

# This is the command used to build every .oct file.
OCT_BUILD = $(MKOCTFILE) $(CXXFLAGS)

# =========================================================================
# File lists (all .oct targets)
# =========================================================================

BOOST_ACCUMULATORS = \
  boost_accumulators_coherent_tail_mean \
  boost_accumulators_count \
  boost_accumulators_covariance \
  boost_accumulators_density \
  boost_accumulators_error_of_mean \
  boost_accumulators_extended_p_square \
  boost_accumulators_extended_p_square_quantile_and_variants \
  boost_accumulators_kurtosis \
  boost_accumulators_max \
  boost_accumulators_mean_and_variants \
  boost_accumulators_median_and_variants \
  boost_accumulators_min \
  boost_accumulators_moment \
  boost_accumulators_non_coherent_tail_mean \
  boost_accumulators_non_coherent_weighted_tail_mean \
  boost_accumulators_p_square_cumulative_distribution \
  boost_accumulators_p_square_quantile_and_variants \
  boost_accumulators_peaks_over_threshold_and_variants \
  boost_accumulators_pot_quantile_and_variants \
  boost_accumulators_pot_tail_mean \
  boost_accumulators_skewness \
  boost_accumulators_sum_and_variants \
  boost_accumulators_tail \
  boost_accumulators_tail_quantile \
  boost_accumulators_tail_variate \
  boost_accumulators_tail_variate_means_and_variants \
  boost_accumulators_variance_and_variants \
  boost_accumulators_weighted_covariance \
  boost_accumulators_weighted_density \
  boost_accumulators_weighted_extended_p_square \
  boost_accumulators_weighted_kurtosis \
  boost_accumulators_weighted_mean_and_variants \
  boost_accumulators_weighted_median_and_variants \
  boost_accumulators_weighted_moment \
  boost_accumulators_weighted_p_square_cumulative_distribution \
  boost_accumulators_weighted_p_square_quantile_and_variants \
  boost_accumulators_weighted_peaks_over_threshold_and_variants \
  boost_accumulators_weighted_skewness \
  boost_accumulators_weighted_sum_and_variants \
  boost_accumulators_weighted_tail_quantile \
  boost_accumulators_weighted_tail_variate_means_and_variants \
  boost_accumulators_weighted_variance_and_variants

BOOST_CHRONO = \
  boost_chrono_duration_count \
  boost_chrono_duration_add \
  boost_chrono_duration_minus \
  boost_chrono_duration_multiply \
  boost_chrono_duration_divide_by_times \
  boost_chrono_duration_divide_by_duration \
  boost_chrono_duration_remainder_by_times \
  boost_chrono_duration_remainder_by_duration \
  boost_chrono_duration_eq \
  boost_chrono_duration_ne \
  boost_chrono_duration_lt \
  boost_chrono_duration_le \
  boost_chrono_duration_gt \
  boost_chrono_duration_ge \
  boost_chrono_duration_cast \
  boost_chrono_system_clock_now \
  boost_chrono_steady_clock_now \
  boost_chrono_high_resolution_clock_now \
  boost_process_cpu_clock_now \
  boost_thread_clock_now \
  boost_chrono_system_clock_duration \
  boost_chrono_steady_clock_duration \
  boost_chrono_high_resolution_clock_duration \
  boost_process_cpu_clock_duration \
  boost_thread_clock_duration \
  boost_time_fmt_duration \
  boost_time_fmt_system_clock

BOOST_DATE = \
  boost_date_get_year \
  boost_date_get_month \
  boost_date_get_day \
  boost_date_get_year_month_day \
  boost_date_get_day_of_week \
  boost_date_get_day_of_year \
  boost_date_get_end_of_month \
  boost_date_is_infinity \
  boost_date_is_neg_infinity \
  boost_date_is_pos_infinity \
  boost_date_is_not_a_date \
  boost_date_is_special \
  boost_date_modjulian_day \
  boost_date_julian_day \
  boost_date_week_number \
  boost_date_end_of_month \
  boost_date_to_simple_string \
  boost_date_to_iso_string \
  boost_date_to_iso_extended_string \
  boost_date_eq \
  boost_date_ne \
  boost_date_gt \
  boost_date_lt \
  boost_date_ge \
  boost_date_le \
  boost_date_add_days \
  boost_date_minus_days \
  boost_date_minus_date \
  boost_date_to_tm \
  boost_date_from_tm \
  boost_date_period_shift \
  boost_date_period_expand \
  boost_date_period_begin \
  boost_date_period_last \
  boost_date_period_end \
  boost_date_period_length \
  boost_date_period_is_null \
  boost_date_period_contains_date \
  boost_date_period_contains_date_period \
  boost_date_period_intersection \
  boost_date_period_is_adjacent \
  boost_date_period_is_after \
  boost_date_period_is_before \
  boost_date_period_merge \
  boost_date_period_span \
  boost_date_span_to_simple_string \
  boost_date_period_eq \
  boost_date_period_ne \
  boost_date_period_gt \
  boost_date_period_lt \
  boost_date_gregorian_day_number \
  boost_date_gregorian_end_of_month_day \
  boost_date_gregorian_from_day_number \
  boost_date_gregorian_is_leap_year

BOOST_PTIME = \
  boost_ptime_date \
  boost_ptime_time_of_day \
  boost_ptime_is_infinity \
  boost_ptime_is_neg_infinity \
  boost_ptime_is_pos_infinity \
  boost_ptime_is_not_a_date_time \
  boost_ptime_is_special \
  boost_ptime_to_simple_string \
  boost_ptime_to_iso_string \
  boost_ptime_to_iso_extended_string \
  boost_ptime_eq \
  boost_ptime_ne \
  boost_ptime_gt \
  boost_ptime_lt \
  boost_ptime_ge \
  boost_ptime_le \
  boost_ptime_add_days \
  boost_ptime_minus_days \
  boost_ptime_add_time_duration \
  boost_ptime_minus_time_duration \
  boost_ptime_minus_ptime \
  boost_ptime_to_tm \
  boost_ptime_from_tm \
  boost_ptime_from_time_t \
  boost_ptime_from_ftime

BOOST_TIME_DURATION = \
  boost_time_duration_to_tm \
  boost_time_duration_hours \
  boost_time_duration_minutes \
  boost_time_duration_seconds \
  boost_time_duration_total_seconds \
  boost_time_duration_total_milliseconds \
  boost_time_duration_total_microseconds \
  boost_time_duration_total_nanoseconds \
  boost_time_duration_fractional_seconds \
  boost_time_duration_is_negative \
  boost_time_duration_is_zero \
  boost_time_duration_is_positive \
  boost_time_duration_invert_sign \
  boost_time_duration_abs \
  boost_time_duration_resolution \
  boost_time_duration_num_fractional_digits \
  boost_time_duration_ticks_per_second \
  boost_time_duration_ticks \
  boost_time_duration_unit \
  boost_time_duration_is_neg_infinity \
  boost_time_duration_is_pos_infinity \
  boost_time_duration_is_not_a_date_time \
  boost_time_duration_is_special \
  boost_time_duration_to_simple_string \
  boost_time_duration_to_iso_string \
  boost_time_duration_eq \
  boost_time_duration_ne \
  boost_time_duration_gt \
  boost_time_duration_lt \
  boost_time_duration_ge \
  boost_time_duration_le \
  boost_time_duration_plus_time_duration \
  boost_time_duration_minus_time_duration \
  boost_time_duration_divide_by_days \
  boost_time_duration_multiply_by_days

BOOST_TIME_PERIOD = \
  boost_time_period_begin \
  boost_time_period_last \
  boost_time_period_end \
  boost_time_period_length \
  boost_time_period_is_null \
  boost_time_period_contains_ptime \
  boost_time_period_contains_time_period \
  boost_time_period_intersects \
  boost_time_period_intersection \
  boost_time_period_merge \
  boost_time_period_span \
  boost_time_period_to_simple_string \
  boost_time_period_eq \
  boost_time_period_ne \
  boost_time_period_gt \
  boost_time_period_lt \
  boost_time_period_ge \
  boost_time_period_le

BOOST_PROPERTY_TREE = \
  boost_info_file_to_ini_file \
  boost_ini_file_to_info_file \
  boost_info_file_to_json_file \
  boost_json_file_to_info_file \
  boost_info_file_to_xml_file \
  boost_xml_file_to_info_file \
  boost_ini_file_to_json_file \
  boost_json_file_to_ini_file \
  boost_ini_file_to_xml_file \
  boost_xml_file_to_ini_file \
  boost_json_file_to_xml_file \
  boost_xml_file_to_json_file \
  boost_save_as_info_file \
  boost_save_as_ini_file \
  boost_save_as_json_file \
  boost_save_as_xml_file \
  boost_parse_from_info_file \
  boost_parse_from_ini_file \
  boost_parse_from_json_file \
  boost_parse_from_xml_file

# Boost.Thread requires -lboost_thread
BOOST_THREAD = \
  boost_multi_thread_run_octave_file \
  boost_multi_thread_run_octave_files \
  boost_multi_thread_call_octave_function \
  boost_multi_thread_call_octave_functions \
  boost_multi_thread_eval_octave_expression \
  boost_multi_thread_eval_octave_expressions \
  boost_multi_thread_feval_octave_expression \
  boost_multi_thread_feval_octave_expressions

# All .oct targets
OCT_FILES = $(addsuffix .oct, $(BOOST_ACCUMULATORS) $(BOOST_CHRONO) \
             $(BOOST_DATE) $(BOOST_PTIME) $(BOOST_TIME_DURATION) \
             $(BOOST_TIME_PERIOD) $(BOOST_PROPERTY_TREE) $(BOOST_THREAD))

# =========================================================================
# Build rules
# =========================================================================

.PHONY: all clean install

all: $(OCT_FILES)

# Pattern rules for header-only Boost modules (no link libraries needed)
boost_accumulators_%.oct: boost_accumulators_%.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

# Boost.Chrono requires -lboost_chrono (clock::now() is NOT header-only on Linux)
BOOST_CHRONO_LIBS = -lboost_chrono

boost_chrono_%.oct: boost_chrono_%.cc
	$(OCT_BUILD) $(BOOST_CHRONO_LIBS) $< $(BOOST_CPPFLAGS)

boost_process_cpu_clock_%.oct: boost_process_cpu_clock_%.cc
	$(OCT_BUILD) $(BOOST_CHRONO_LIBS) $< $(BOOST_CPPFLAGS)

boost_thread_clock_%.oct: boost_thread_clock_%.cc
	$(OCT_BUILD) $(BOOST_CHRONO_LIBS) $< $(BOOST_CPPFLAGS)

boost_time_fmt_%.oct: boost_time_fmt_%.cc
	$(OCT_BUILD) $(BOOST_CHRONO_LIBS) $< $(BOOST_CPPFLAGS)

boost_date_%.oct: boost_date_%.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_ptime_%.oct: boost_ptime_%.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_time_duration_%.oct: boost_time_duration_%.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_time_period_%.oct: boost_time_period_%.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

# Property tree files (explicit rules since names don't follow a single pattern)
boost_info_file_to_ini_file.oct: boost_info_file_to_ini_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_ini_file_to_info_file.oct: boost_ini_file_to_info_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_info_file_to_json_file.oct: boost_info_file_to_json_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_json_file_to_info_file.oct: boost_json_file_to_info_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_info_file_to_xml_file.oct: boost_info_file_to_xml_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_xml_file_to_info_file.oct: boost_xml_file_to_info_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_ini_file_to_json_file.oct: boost_ini_file_to_json_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_json_file_to_ini_file.oct: boost_json_file_to_ini_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_ini_file_to_xml_file.oct: boost_ini_file_to_xml_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_xml_file_to_ini_file.oct: boost_xml_file_to_ini_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_json_file_to_xml_file.oct: boost_json_file_to_xml_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_xml_file_to_json_file.oct: boost_xml_file_to_json_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_save_as_info_file.oct: boost_save_as_info_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_save_as_ini_file.oct: boost_save_as_ini_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_save_as_json_file.oct: boost_save_as_json_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_save_as_xml_file.oct: boost_save_as_xml_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_parse_from_info_file.oct: boost_parse_from_info_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_parse_from_ini_file.oct: boost_parse_from_ini_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_parse_from_json_file.oct: boost_parse_from_json_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

boost_parse_from_xml_file.oct: boost_parse_from_xml_file.cc
	$(OCT_BUILD) $< $(BOOST_CPPFLAGS)

# Boost.Thread requires linking with the Boost thread library
boost_multi_thread_%.oct: boost_multi_thread_%.cc
	$(OCT_BUILD) -lboost_thread $< $(BOOST_CPPFLAGS)

# =========================================================================
# Clean
# =========================================================================

clean:
	$(RM) *.oct *.o octave-core octave-workspace *~

# =========================================================================
# Install into Octave package structure
# =========================================================================
INSTALL_DIR ?= $(DESTDIR)

install: all
	@echo "Installing .oct files..."
	@if test -n "$(INSTALL_DIR)"; then \
	  cp $(OCT_FILES) "$(INSTALL_DIR)/"; \
	  echo "Installed to $(INSTALL_DIR)"; \
	else \
	  echo "Set INSTALL_DIR to install, or use 'make DESTDIR=/path install'"; \
	  echo "Alternatively, run 'make' then 'cp *.oct /path/to/octave/search/path/'"; \
	fi
