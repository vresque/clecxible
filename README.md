# CLECXIBLE

C-Language, with extensions.


## Plan
A C(11) compiler with some extensions, as well as the ability to add (simple) extensions via `.c-ext` files

### A simple extension
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

This will then be compiled into a shared object (`*.c-ext-lib`) and dynamically linked at runtime.

All that this does is define an extension, called "Namespaces", which matches `namespace IDENT BLOCK` and changes the **exported** name of all items within `BLOCK` to `namespace_name_item_name`.


#### Example
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
