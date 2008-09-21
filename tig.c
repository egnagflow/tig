static bool prompt_yesno(const char *prompt);
	"git log --no-color --topo-order --parents --pretty=raw %s 2>/dev/null"
	REQ_(STATUS_CHECKOUT,	"Checkout file"), \
static int opt_author_cols		= AUTHOR_COLS-1;
static enum request
	enum request request = REQ_VIEW_MAIN;
		return REQ_VIEW_PAGER;
		return REQ_VIEW_MAIN;
		return REQ_VIEW_STATUS;
		return REQ_VIEW_BLAME;
		request = REQ_VIEW_DIFF;
		request = subcommand[0] == 'l' ? REQ_VIEW_LOG : REQ_VIEW_DIFF;
		string_copy(opt_cmd, "git log --no-color --pretty=raw --parents");
			return REQ_NONE;
			return REQ_NONE;
	return request;
	{ '!',		REQ_STATUS_CHECKOUT },
static int
parse_int(const char *s, int default_value, int min, int max)
{
	int value = atoi(s);

	return (value < min || value > max) ? default_value : value;
}

		opt_num_interval = parse_int(argv[2], opt_num_interval, 1, 1024);
		return OK;
	}

	if (!strcmp(argv[0], "author-width")) {
		opt_author_cols = parse_int(argv[2], opt_author_cols, 0, 1024);
		opt_tab_size = parse_int(argv[2], opt_tab_size, 1, 1024);
end_update(struct view *view, bool force)
	while (!view->ops->read(view, NULL))
		if (!force)
			return;
	if (ferror(view->pipe) && errno != 0) {
		end_update(view, TRUE);
		end_update(view, FALSE);
	end_update(view, TRUE);
	if (view->pipe)
		end_update(view, TRUE);

static void
run_confirm(const char *cmd, const char *prompt)
{
	if (prompt_yesno(prompt)) {
		system(cmd);
	}
}

		    view == VIEW(REQ_VIEW_MAIN) ||
			end_update(view, TRUE);
	    draw_field(view, LINE_MAIN_AUTHOR, author, opt_author_cols, TRUE))
static bool
status_checkout(struct view *view)
{
	struct line *line = &view->line[view->lineno];

	assert(view->lines);

	if (!line->data || line->type != LINE_STAT_UNSTAGED) {
		/* This should work even for the "On branch" line. */
		if (line < view->line + view->lines && !line[1].data) {
			report("Nothing to checkout");
		} else if (line->type == LINE_STAT_UNTRACKED) {
			report("Cannot checkout untracked files");
		} else if (line->type == LINE_STAT_STAGED) {
			report("Cannot checkout staged files");
		} else {
			report("Cannot checkout multiple files");
		}
		return FALSE;

	} else {
		struct status *status = line->data;
		char cmd[SIZEOF_STR];
		char file_sq[SIZEOF_STR];

		if (sq_quote(file_sq, 0, status->old.name) < sizeof(file_sq) &&
		    string_format(cmd, "git checkout %s%s", opt_cdup, file_sq)) {
			run_confirm(cmd, "Are you sure you want to overwrite any changes?");
		}

		return TRUE;
	}
}

	case REQ_STATUS_CHECKOUT:
		if (!status_checkout(view))
			return REQ_NONE;
		break;

static void
clear_rev_graph(struct rev_graph *graph)
{
	graph->boundary = 0;
	graph->size = graph->pos = 0;
	graph->commit = NULL;
	memset(graph->parents, 0, sizeof(*graph->parents));
}

	clear_rev_graph(graph);
	    draw_field(view, LINE_MAIN_AUTHOR, commit->author, opt_author_cols, TRUE))
		int i;

		if (view->lines > 0) {
			commit = view->line[view->lines - 1].data;
			if (!*commit->author) {
				view->lines--;
				free(commit);
				graph->commit = NULL;
			}
		}

		for (i = 0; i < ARRAY_SIZE(graph_stacks); i++)
			clear_rev_graph(&graph_stacks[i]);
	switch (request) {
	case REQ_ENTER:
		break;
	case REQ_REFRESH:
		string_copy(opt_cmd, view->cmd);
		open_view(view, REQ_VIEW_MAIN, OPEN_RELOAD);
		break;
	default:
	}
static bool
prompt_yesno(const char *prompt)
{
	enum { WAIT, STOP, CANCEL  } status = WAIT;
	bool answer = FALSE;

	while (status == WAIT) {
		struct view *view;
		int i, key;

		input_mode = TRUE;

		foreach_view (view, i)
			update_view(view);

		input_mode = FALSE;

		mvwprintw(status_win, 0, 0, "%s [Yy]/[Nn]", prompt);
		wclrtoeol(status_win);

		/* Refresh, accept single keystroke of input */
		key = wgetch(status_win);
		switch (key) {
		case ERR:
			break;

		case 'y':
		case 'Y':
			answer = TRUE;
			status = STOP;
			break;

		case KEY_ESC:
		case KEY_RETURN:
		case KEY_ENTER:
		case KEY_BACKSPACE:
		case 'n':
		case 'N':
		case '\n':
		default:
			answer = FALSE;
			status = CANCEL;
		}
	}

	/* Clear the status window */
	status_empty = FALSE;
	report("");

	return answer;
}

	request = parse_options(argc, argv);
	if (request == REQ_NONE)
	if (!opt_git_dir[0] && request != REQ_VIEW_PAGER)
					request = REQ_VIEW_DIFF;
					request = REQ_VIEW_PAGER;

				/* Always reload^Wrerun commands from the prompt. */
				open_view(view, request, OPEN_RELOAD);