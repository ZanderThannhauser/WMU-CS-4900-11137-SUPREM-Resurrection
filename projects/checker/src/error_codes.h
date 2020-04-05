

enum
{
	/* misc: */
	e_success = 0,
	e_systemcall_failed,
	
	/* process_args: */
	e_unknown_flag,
	e_help_flag,
	e_flag_requires_argument,
	e_flag_not_enough_information,
	e_input_and_pattern_cant_be_same_path,
	
	/* read_pattern: */
	e_read_char_EOF,
	e_read_pattern_file_empty,
	e_read_pattern_syntax_error,
	e_read_exact_string_unexpected_EOF,
	e_read_number_range_unexpected_EOF,
	e_read_number_range_invaild_format,
	
	/* check_match: */
	e_check_input_doesnt_match_pattern,
	
};
