#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef int8_t* fsm_id;

typedef enum {
	FSM_STATE_ALLOCATED,
	FSM_STATE_FREED,
	FSM_STATE_ERROR
} fsm_state;

typedef enum {
	FSM_ALPHABET_FREE,
	FSM_ALPHABET_MALLOC,
} fsm_alphabet;

// finite state machine
typedef struct {
	fsm_id id;
	fsm_state state;
} fsm;


// FSM list type
typedef struct fsm_list_node{
	fsm *fsm;
	struct fsm_list_node *next;
} fsm_list_node;

fsm_list_node *fsm_list = NULL;

void __INSTR_fsm_list_append(fsm_list_node *node) {
	fsm_list_node *cur = fsm_list;

	while((cur) && (cur->next)) {
	  cur = cur->next;
	}

	if (cur == NULL) {
		cur = node;
		fsm_list = node;
	} else {
		cur->next = node;
	}
}

fsm* __INSTR_fsm_create(fsm_id id, fsm_state state) {
	fsm *new_fsm = (fsm *) malloc(sizeof(fsm));
	new_fsm->id = id;
	new_fsm->state = state;

	fsm_list_node *node = (fsm_list_node *) malloc(sizeof(fsm_list_node));
	node->next = NULL;
	node->fsm = new_fsm;

	__INSTR_fsm_list_append(node);

	return new_fsm;
}

fsm* __INSTR_fsm_list_search(fsm_id id) {
  	fsm_list_node *cur = fsm_list;

  	while(cur) {
  	if ((*cur->fsm).id == id) {
			return cur->fsm;
	}
	  cur = cur->next;
	}

	return NULL;
}

// FSM manipulation

fsm_state fsm_transition_table[3][2] = {{ FSM_STATE_FREED, FSM_STATE_ALLOCATED},
                                        { FSM_STATE_ERROR, FSM_STATE_ALLOCATED},
                                        { FSM_STATE_ERROR, FSM_STATE_ERROR }};

void __INSTR_fsm_change_state(fsm_id id, fsm_alphabet action) {

	// there is no FSM for NULL
	if (id == 0) {
		return;
	}

	fsm *m = __INSTR_fsm_list_search(id);
	if (m != NULL) {
		m->state = fsm_transition_table[m->state][action];
	} else {
		if (action == FSM_ALPHABET_FREE) {
			assert(0 && "free on non-allocated memory");
			__VERIFIER_error();
		}
		m = __INSTR_fsm_create(id, FSM_STATE_ALLOCATED);
	}

	if (m->state == FSM_STATE_ERROR) {
	        assert(0 && "double free");
		__VERIFIER_error();
	}
}

void __INSTR_fsm_destroy(fsm_id id) {
    fsm_list_node *cur = fsm_list;

    if(cur && cur->fsm->id == id) {
        fsm_list_node *newHead = cur->next;
        free(cur->fsm);
        free(cur);
        fsm_list = newHead;
        return;
    }

  	while((cur) && (cur->next)) {
  	  if ((*cur->next->fsm).id == id) {
            fsm_list_node *tmp = cur->next->next;
            free(cur->next->fsm);
            free(cur->next);
	    cur->next = tmp;
	    return;
	  }
	  cur = cur->next;
	}

}

void __INSTR_fsm_list_destroy() {
    fsm_list_node *cur = fsm_list;

    while(cur) {
        fsm_list_node *tmp = cur->next;
        free(cur->fsm);
        free(cur);
        cur = tmp;
    }

}

void __INSTR_fsm_list_destroy_checked() {
    fsm_list_node *cur = fsm_list;

    while(cur) {
        fsm_list_node *tmp = cur->next;
	if (cur->fsm->state == FSM_STATE_ALLOCATED){
		assert(0 && "memory leak detected");
		__VERIFIER_error();
	}

        free(cur->fsm);
        free(cur);
        cur = tmp;
    }
}
