# CLECXIBLE

C-Language, with extensions.


## Plan
A C(11) compiler with some extensions, as well as the ability to add (simple) extensions via `.c-ext` files

I am still undecided, which way of adding extensions would be better

### Option A
Example of a `.c-ext` file, which adds namespaces
```c
(set-name "Namespaces")
(set-description "Introduces namespaces to C")
(set-version "0.1")
(runs-after parsing)
(assert-in-domain module) ;; Asserts that it is either in another namespace or in the global scope, but not in functions, etc
(set-pattern
	namespace ?name ?block 
)
(set-param-type ?name ident)
;; Another possible option here
;; (set-param-optional ?block)
(set-param-type ?block block)

(activate (?name ?block) (
	(for_item_in_block it block (
		(push (concat_idents ?name (get-name it))) ;; No local variables, just push, pop, dup and clear
		(set-exported-name it (pop))
		(set-domain it module)
	))
))

```

All that this does is define an extension, called "Namespaces", which matches `namespace IDENT BLOCK` and changes the **exported** name of all items within `BLOCK` to `namespace_name_item_name`.

### Option B
```c
#include <clecxible.h>

// stream is going to look like this:
// CustomKeyWord Ident Block
void activate(struct ParsedStream* stream) {
	struct CustomKeywordNode* kw = advance_expect(stream, NODE_CUSTOM_KEYWORD);
	struct IdentNode* ident = advance_expect(stream, NODE_IDENT);
	struct BlockNode* block = advance_expect(stream, NODE_BLOCK);
	
	FOR_ALL_IN_BLOCK(it, block) {
		struct Ident name = concat_idents(ident->id, it->name);
		it->exported_name = name;
	}
}

INTRODUCE_TOKEN(
	TOKEN_KW_NAMESPACE,
	"namespace",
	DOMAIN_MODULE)

struct TokenStream* pattern() {
	return TOKENSTREAM(TOKEN_KW_NAMESPACE, TOKEN_IDENT, BLOCK);
}

NEW_EXTENSION(
	activate, // Activation function
	pattern, // Pattern function
	"Namespaces", // Name
	"Adds namespaces to C", // Description
	"0.1", // Version
	// Custom Keywords
	TOKEN_KW_NAMESPACE,
);
```

This would then be compiled into a shared object and dynamically linked to at runtime.

Pros:
- C-Syntax
- One parser can do it all
Cons:
- Would force developers to either include this binary in their repository, or to force devs to get it on their local system

### Example
```c
namespace abc {
	void hello();
}
```

In this case, `hello` would be exported as `abc_hello`.



## Activating an Extension

There are two ways to activate an extension.

### Via C-Files
```c
#extension <namespaces.c-ext> // Searches the global c-ext dir
```

### Via the CLI (planned)
```bash
clecxc --extension "g:namespaces"
```

## Do extensions not destroy the purpose of C?
Extensions should not be thought of as adding features, but much rather as **very** powerful macros.
