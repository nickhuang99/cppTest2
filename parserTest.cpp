//============================================================================
// Name        : parserTest.cpp
// Author      : nick huang
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include "glimits.h"
#include "auto-host.h"
#include "system.h"
#include "ansidecl.h"
#include "libiberty.h"
#include "cpplib.h"

#include "coretypes.h"
#include "c-family/c-common.h"
#include "incpath.h"
#include "intl.h"
#include "diagnostic.h"
#include "options.h"
#include "toplev.h"
#include "context.h"
#include "langhooks.h"
#include "pass_manager.h"
#include "cgraph.h"
#include "insn-constants.h"
#include "config/i386/i386.h"
#include "target.h"
#include "debug.h"
#include "opts-diagnostic.h"
#include "output.h"

//#include "cgraph.h"
using namespace std;

extern void c_parse_file (void);
extern cpp_reader *parse_in;
//extern symbol_table symtab;
extern void c_cpp_builtins (cpp_reader *pfile);
//extern void init_ttree (void);
//extern void init_ggc (void);
extern bool cxx_init (void);
extern void init_derived_machine_modes();
extern enum cxx_dialect cxx_dialect;
extern char flag_dump_macros;
//extern char* main_input_filename;
struct Token_Name
{
	cpp_ttype t;
	unsigned char* name;
};
#define OP(e, s) { CPP_##e,    (unsigned char*) #s },
#define TK(e, s) { CPP_##e,    (unsigned char*) #e },
static const struct Token_Name token_names[N_TTYPES] = { TTYPE_TABLE };
#undef OP
#undef TK
#define TOKEN_NAME(token) (token_names[(token)->type].name)

static size_t round_alloc_size (size_t s)
{
  return s;
}

bool diagnostic_cb (cpp_reader *, enum cpp_diagnostic_level errtype,
	       enum cpp_warning_reason, rich_location *richloc,
	       const char *msg, va_list *ap)
{
  const line_map_ordinary *map;
  location_t location = richloc->get_loc ();
  linemap_resolve_location (line_table, location, LRK_SPELLING_LOCATION, &map);
  expanded_location loc = linemap_expand_location (line_table, map, location);
  fprintf (stderr, "%s:%d:%d %s: ", loc.file, loc.line, loc.column,
	   (errtype == CPP_DL_WARNING) ? "warning" : "error");
  vfprintf (stderr, msg, *ap);
  fprintf (stderr, "\n");
  FILE *f = fopen (loc.file, "r");
  if (f)
    {
      char buf[128];
      while (loc.line > 0)
	{
	  if (!fgets (buf, 128, f))
	    goto notfound;
	  if (buf[strlen (buf) - 1] != '\n')
	    {
	      if (loc.line > 1)
		loc.line++;
	    }
	  loc.line--;
	}
      fprintf (stderr, "%s", buf);
      for (int i = 0; i < loc.column - 1; ++i)
	fputc (' ', stderr);
      fputc ('^', stderr);
      fputc ('\n', stderr);
notfound:
      fclose (f);
    }

  if (errtype == CPP_DL_FATAL)
    exit (1);
  return false;
}
const char* my_missing_header(cpp_reader *parse_in, const char *header, cpp_dir ** ppdir)
{
//	cout<<"missing header:"<<header<<endl<<"dir:"<<(*ppdir)->name<<endl;
	return NULL;
}

void cb_include (cpp_reader * parse_in, location_t loc, const unsigned char * dirName,
		   const char * fname, int angle_brackets, const cpp_token **ptoken)
{
	cerr<<"loc:"<<loc<<endl<<"dirname:"<<dirName<<endl<<"fname:"<<fname<<endl<<"angle_bracket:"<<angle_brackets<<endl;

//	cpp_buffer* pbuffer=cpp_get_buffer(parse_in);
//	_cpp_file*pfile=cpp_get_file(pbuffer);
//	cout<<"path:"<<cpp_get_path(pfile)<<endl;
	expanded_location exploc=linemap_client_expand_location_to_spelling_point(loc, LOCATION_ASPECT_CARET);
	cerr<<"file:"<<exploc.file<<endl<<"line:"<<exploc.line<<":column"<<exploc.column<<endl<<"************************"<<endl;

	//cpp_warning_with_line(parse_in, CPP_W_NONE, loc, 0, "warning from nick");
}
//extern void define_language_independent_builtin_macros (cpp_reader *pfile);
//extern void parse_basever (int *major, int *minor, int *patchlevel);
//extern void c_stddef_cpp_builtins(void);
int test()
{
	hex_init ();

	/* Unlock the stdio streams.  */
	unlock_std_streams ();

	gcc_init_libintl ();
	diagnostic_initialize (global_dc, 0);
	cxx_dialect = cxx17;
	init_ggc ();
	init_stringpool ();
	init_ttree ();
    init_adjust_machine_modes ();
    init_derived_machine_modes ();
//	symtab = new (ggc_alloc <symbol_table> ()) symbol_table ();


	line_table = XCNEW (class line_maps);
	linemap_init (line_table, BUILTINS_LOCATION);
	line_table->reallocator = xrealloc;
	line_table->round_alloc_size = round_alloc_size;
	parse_in=cpp_create_reader(CLK_CXX17, nullptr, line_table);
	if (parse_in)
	{
		cpp_options * poption=cpp_get_options(parse_in);
		poption->debug=1;
		poption->cplusplus_comments=1;

		char*path=strdup("/home/nick/Downloads/gcc-10.2.0/gcc-10.2.0-debug-libcpp-install/lib/gcc/x86_64-pc-linux-gnu/10.2.0/plugin/include");

		add_path(path, INC_BRACKET, 1, 1);
		path=strdup("/usr/include/x86_64-linux-gnu");
		add_path(path, INC_SYSTEM, 1, 1);
		//register_include_chains(parse_in, "/tmp,/usr/include/x86_64-linux-gnu", nullptr, nullptr, 1, 1, 1);
		register_include_chains(parse_in, nullptr, nullptr, nullptr, 1, 1, 1);
//		cpp_dir *dir = XCNEW (cpp_dir);
//		dir->name = ASTRDUP ("/home/nick/eclipse-2021/cppTest/src");
//		cpp_set_include_chains (parse_in, dir, NULL, true);
		main_input_filename=strdup("/home/nick/eclipse-2021/parserTest/src/parserTest.cpp");
		const char* ptr=cpp_read_main_file(parse_in, "/home/nick/eclipse-2021/parserTest/src/parserTest.cpp");
		//const char* ptr=cpp_read_main_file(parse_in, "/tmp/test.c");
		if (ptr && cxx_init())
		{
			init_eh ();
			cpp_init_iconv(parse_in);
			linemap_add (line_table, LC_RENAME, 0, _("<built-in>"), 0);
			cpp_force_token_locations (parse_in, BUILTINS_LOCATION);
			cpp_init_builtins (parse_in, 1);
			c_cpp_builtins (parse_in);

			const line_map_ordinary *cmd_map
				= linemap_check_ordinary (linemap_add (line_table, LC_RENAME, 0, _("<command-line>"), 0));

			/* All command line defines must have the same location.  */
			cpp_force_token_locations (parse_in, cmd_map->start_location);

			cpp_stop_forcing_token_locations (parse_in);
			cpp_init_iconv(parse_in);
			cerr<<"successful!"<<ptr<<endl;
			cpp_callbacks*pCallback=cpp_get_callbacks(parse_in);
			pCallback->include=cb_include;
			pCallback->diagnostic=diagnostic_cb;
			pCallback->missing_header=my_missing_header;
			cpp_set_callbacks(parse_in, pCallback);
			unsigned char buffer[1024];
			const cpp_token*token=nullptr;
			token=cpp_get_token(parse_in);
			while (token && token->type!=CPP_EOF)
			{
				if (token->type != CPP_PADDING)
				{
					cout<<"token-type:"<<TOKEN_NAME(token)<<" of : ";
					unsigned char*ptr=cpp_spell_token(parse_in, token, buffer, true);
					cout<<string((const char*)(buffer), ptr-buffer)<<endl;
				}
				token=cpp_get_token(parse_in);
			}

		}
	}
	return 0;
}
static void cpp_init();
static void general_init (const char *argv0);
extern void init_options_once ();
extern void init_opts_obstack ();
extern void init_options_struct (struct gcc_options *opts, struct gcc_options *opts_set);
extern struct gcc_target targetm;
static bool standard_type_bitsize (int bitsize)
{
  /* As a special exception, we always want __int128 enabled if possible.  */
  if (bitsize == 128)
    return false;
  if (bitsize == CHAR_TYPE_SIZE
      || bitsize == SHORT_TYPE_SIZE
      || bitsize == INT_TYPE_SIZE
      || bitsize == LONG_TYPE_SIZE
      || bitsize == LONG_LONG_TYPE_SIZE)
    return true;
  return false;
}
void init1()
{
    for (int i = 0; i < NUM_INT_N_ENTS; i ++)
	if (targetm.scalar_mode_supported_p (int_n_data[i].m)
	    && ! standard_type_bitsize (int_n_data[i].bitsize))
	  int_n_enabled_p[i] = true;
	else
	  int_n_enabled_p[i] = false;

      /* Initialize mpfrs exponent range.  This is important to get
         underflow/overflow in a reasonable timeframe.  */
      machine_mode mode;
      int min_exp = -1;
      int max_exp = 1;
      FOR_EACH_MODE_IN_CLASS (mode, MODE_FLOAT)
	if (SCALAR_FLOAT_MODE_P (mode))
	  {
	    const real_format *fmt = REAL_MODE_FORMAT (mode);
	    if (fmt)
	      {
		/* fmt->emin - fmt->p + 1 should be enough but the
		   back-and-forth dance in real_to_decimal_for_mode we
		   do for checking fails due to rounding effects then.  */
		if ((fmt->emin - fmt->p) < min_exp)
		  min_exp = fmt->emin - fmt->p;
		if (fmt->emax > max_exp)
		  max_exp = fmt->emax;
	      }
	  }
      /* E.g. mpc_norm assumes it can square a number without bothering with
	 with range scaling, so until that is fixed, double the minimum
	 and maximum exponents, plus add some buffer for arithmetics
	 on the squared numbers.  */
//      if (mpfr_set_emin (2 * (min_exp - 1))
//	  || mpfr_set_emax (2 * (max_exp + 1)))
//	sorry ("mpfr not configured to handle all floating modes");
}

//static int
//dump_macro (cpp_reader *pfile, cpp_hashnode *node, void *v ATTRIBUTE_UNUSED)
//{
//  if (cpp_user_macro_p (node))
//    {
//      cout<< "#define "<< cpp_macro_definition (pfile, node)<<endl;
////      print.printed = false;
////      print.src_line++;
//    }
//
//  return 1;
//}

void define_cb(cpp_reader *cpp, location_t loc, cpp_hashnode *hash)
{
	cout<<cpp_macro_definition(cpp, hash)<<endl;
}

void test1()
{
	// main

	stack_limit_increase (64 * 1024 * 1024);
	general_init("/home/nick/Downloads/gcc-10.2.0/gcc-10.2.0-debug-libcpp-install/libexec/gcc/x86_64-pc-linux-gnu/10.2.0/cc1plus");
	targetm.target_option.override ();
	init_options_once ();
	init_opts_obstack ();
	init_options_struct (&global_options, &global_options_set);
	lang_hooks.init_options_struct (&global_options);
	init_ggc_heuristics ();
	// do_compile
	debug_hooks = &do_nothing_debug_hooks;
	init_adjust_machine_modes ();
	init_derived_machine_modes ();
	init1();
	cxx_dialect = cxx17;
	imultiarch="x86_64-linux-gnu";
	main_input_filename=strdup("/home/nick/eclipse-2021/parserTest/src/parserTest.cpp");
	//lang_dependent_init(main_input_filename);
	cpp_init();
	FILE* output=fopen("/tmp/nick4.i", "w");
	init_pp_output(output);
	flag_abi_version=14; //latest_abi_version=14;
	flag_preprocess_only=1;
	user_label_prefix="";
	ix86_arch_string="x86_64";
	ix86_tune_string="generic";
	char*path=strdup("/home/nick/Downloads/gcc-10.2.0/gcc-10.2.0-debug-libcpp-install/lib/gcc/x86_64-pc-linux-gnu/10.2.0/plugin/include");
	add_path(path, INC_BRACKET, 0, 1);
	register_include_chains (parse_in, nullptr, nullptr, nullptr, 1, 1, 1);
	//flag_dump_macros='M';
	cpp_callbacks * callback= cpp_get_callbacks(parse_in);
	callback->define=define_cb;
	if (cpp_read_main_file(parse_in, main_input_filename))
	{
		cxx_init();
	}
	fclose(output);
}
int main(int argc, char**argv)
{
//	toplev toplev (NULL, /* external_timer */ true /* init_signals */);
//
//	return toplev.main (argc, argv);
	test1();
}

extern void tree_diagnostics_defaults (diagnostic_context *context);
void debuginfo_early_init (void);
extern struct lang_hooks lang_hooks;
extern gcc::context *g;
extern cpp_options *cpp_opts;
static void general_init (const char *argv0)
{
  const char *p;

  p = argv0 + strlen (argv0);
  while (p != argv0 && !IS_DIR_SEPARATOR (p[-1]))
    --p;
  progname = p;

  xmalloc_set_program_name (progname);

  hex_init ();

  /* Unlock the stdio streams.  */
  unlock_std_streams ();

  gcc_init_libintl ();

//  identifier_to_locale_alloc = alloc_for_identifier_to_locale;
  identifier_to_locale_free = ggc_free;

  /* Initialize the diagnostics reporting machinery, so option parsing
     can give warnings and errors.  */
  diagnostic_initialize (global_dc, N_OPTS);
  global_dc->lang_mask = lang_hooks.option_lang_mask ();
  /* Set a default printer.  Language specific initializations will
     override it later.  */
  tree_diagnostics_defaults (global_dc);

  global_dc->show_caret
    = global_options_init.x_flag_diagnostics_show_caret;
  global_dc->show_labels_p
    = global_options_init.x_flag_diagnostics_show_labels;
  global_dc->show_line_numbers_p
    = global_options_init.x_flag_diagnostics_show_line_numbers;
  global_dc->show_cwe
    = global_options_init.x_flag_diagnostics_show_cwe;
  global_dc->path_format
    = (enum diagnostic_path_format)global_options_init.x_flag_diagnostics_path_format;
  global_dc->show_path_depths
    = global_options_init.x_flag_diagnostics_show_path_depths;
  global_dc->show_option_requested
    = global_options_init.x_flag_diagnostics_show_option;
  global_dc->min_margin_width
    = global_options_init.x_diagnostics_minimum_margin_width;
  global_dc->show_column
    = global_options_init.x_flag_show_column;
//  global_dc->internal_error = internal_error_function;
//  global_dc->option_enabled = option_enabled;
  global_dc->option_state = &global_options;
  global_dc->option_name = option_name;
  global_dc->get_option_url = get_option_url;


  /* Initialize the garbage-collector, string pools and tree type hash
     table.  */
  init_ggc ();
  init_stringpool ();
  input_location = UNKNOWN_LOCATION;
  line_table = ggc_alloc<line_maps> ();
  linemap_init (line_table, BUILTINS_LOCATION);
  line_table->reallocator = xrealloc;
  line_table->round_alloc_size = ggc_round_alloc_size;
  line_table->default_range_bits = 5;
  init_ttree ();

  /* Initialize register usage now so switches may override.  */
//  init_reg_sets ();

  /* Create the singleton holder for global state.  This creates the
     dump manager.  */
  g = new gcc::context ();

  /* Allow languages and middle-end to register their dumps before the
     optimization passes.  */
  g->get_dumps ()->register_dumps ();

  /* Create the passes.  */
  g->set_passes (new gcc::pass_manager (g));

  symtab = new (ggc_alloc <symbol_table> ()) symbol_table ();

  statistics_early_init ();
  debuginfo_early_init ();
}

static void cpp_init()
{
  //unsigned int i;
  struct cpp_callbacks *cb;

  g_string_concat_db
    = new (ggc_alloc <string_concat_db> ()) string_concat_db ();

  parse_in = cpp_create_reader (CLK_CXX17, ident_hash, line_table);
  cb = cpp_get_callbacks (parse_in);
  cb->diagnostic = c_cpp_diagnostic;

  cpp_opts = cpp_get_options (parse_in);
 // cpp_opts->dollars_in_ident = DOLLARS_IN_IDENTIFIERS;
  cpp_opts->objc = c_dialect_objc ();
//
//  /* Reset to avoid warnings on internal definitions.  We set it just
//     before passing on command-line options to cpplib.  */
//  cpp_opts->warn_dollars = 0;

//  deferred_opts = XNEWVEC (struct deferred_opt, decoded_options_count);
//  set_std_c17 (false /* ISO */);
//  if (c_language == clk_c)
//    {
//      /* The default for C is gnu17.  */
//      set_std_c17 (false /* ISO */);

//      /* If preprocessing assembly language, accept any of the C-family
//	 front end options since the driver may pass them through.  */
//      for (i = 1; i < decoded_options_count; i++)
//	if (decoded_options[i].opt_index == OPT_lang_asm)
//	  {
//	    accept_all_c_family_options = true;
//	    break;
//	  }
//    }

  /* Set C++ standard to C++14 if not specified on the command line.  */
//  if (c_dialect_cxx ())
//    set_std_cxx14 (/*ISO*/false);

  global_dc->colorize_source_p = true;
}


