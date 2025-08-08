# Copilot Instructions for Lima MUD Project

## Project Overview
This is a Lima MUD (Multi-User Dungeon) project written in LPC (Lars Pensjö C) using the FluffOS driver. Lima is a sophisticated mudlib that provides a complete framework for building text-based virtual worlds with rich gameplay mechanics, object-oriented programming features, and real-time multiplayer interactions.

## Technology Stack
- **Language**: LPC (Lars Pensjö C)
- **Driver**: FluffOS
- **Mudlib**: Lima Mudlib
- **Documentation**: https://docs.limalib.dev/

## LPC Language Specifics

### File Extensions and Types
- `.c` files: LPC source code (objects, rooms, NPCs, items, etc.)
- `.h` files: Header files with #define macros and includes
- Configuration files are typically in `/lib/data/config/`

### LPC Syntax and Conventions
- LPC is C-like but object-oriented with inheritance
- Functions are called "efuns" (external functions) when built into the driver
- Local functions are called "lfuns"
- Variables are declared with type: `int`, `string`, `object`, `mixed`, `mapping`, arrays (`int *`, `string *`)
- Strings use double quotes: `"text"`
- Arrays use `({ element1, element2, element3 })`
- Mappings use `([ key : value, key2 : value2 ])`

### Key LPC Concepts
- **Inheritance**: Objects inherit from standard classes using `inherit CLASS;`
- **create()**: Constructor function called when object is loaded
- **init()**: Called when a living object encounters this object
- **heart_beat()**: Called periodically for objects with heartbeats
- **reset()**: Called periodically to reset object state
- **clone_object()**: Creates instances of objects
- **this_object()**: Returns reference to current object
- **this_player()**: Returns current player object
- **environment()**: Returns object's container/location

## Directory Structure

### Core Directories
- `/lib/` - Main mudlib code
- `/lib/std/` - Standard object classes (room, weapon, armor, etc.)
- `/lib/domains/` - Game areas/domains
- `/lib/cmds/` - Player and wizard commands
- `/lib/daemons/` - System daemons (background services)
- `/lib/include/` - Header files
- `/lib/obj/` - Miscellaneous objects
- `/lib/secure/` - Security-related files
- `/adm/` - Administrative tools
- `/bin/` - Binary executables and utilities

### Important Files
- `/lib/include/global.h` - Global definitions and macros
- `/lib/include/config.h` - Configuration constants
- `/lib/secure/master.c` - Master object (driver interface)
- `/lib/secure/simul_efun.c` - Simulated external functions
- `/lib/lpc-config.json` - LPC language server configuration

## Standard Object Classes
- `STD_ROOM` - Base class for rooms/locations
- `STD_NPC` - Base class for non-player characters
- `STD_WEAPON` - Base class for weapons
- `STD_ARMOUR` - Base class for armor
- `STD_OBJECT` - Base class for general objects
- `STD_USER` - Base class for player characters

## Common LPC Patterns

### Object Creation Template
```lpc
// Example room object
inherit STD_ROOM;

void create() {
    ::create(); // Call parent create()
    set_short("a cozy library");
    set_long("This is a comfortable library with tall bookshelves...");
    set_properties((["light": 2]));
    set_exits(([
        "north": "/domains/town/rooms/main_street",
        "out": "/domains/town/rooms/main_street"
    ]));
}
```

### Function Override Pattern
```lpc
int do_look(string str) {
    if(!str) {
        // Default look behavior
        return ::do_look(str);
    }
    // Custom look behavior
}
```

## Coding Guidelines

### Naming Conventions
- Functions: `snake_case()` (e.g., `set_short()`, `do_damage()`)
- Variables: `snake_case` (e.g., `current_hp`, `max_level`)
- Constants: `UPPER_CASE` (e.g., `MAX_LEVEL`, `DEFAULT_AC`)
- Files: `lowercase.c` or `snake_case.c`

### Documentation Style
Use autodoc comments for functions:
```lpc
/**
 * Calculate damage for an attack
 * @param attacker the object doing the attack
 * @param weapon the weapon being used
 * @return integer damage amount
 */
int calculate_damage(object attacker, object weapon) {
    // implementation
}
```

### Error Handling
- Use `error()` for fatal errors
- Use `return 0` or `return 1` for success/failure
- Check object validity with `objectp()` before use
- Use `catch()` for exception handling

## Common Mudlib Functions

### Object Manipulation
- `clone_object(file)` - Create object instance
- `destruct(object)` - Destroy object
- `move_object(destination)` - Move object
- `environment(object)` - Get object's location

### Player Interaction
- `write(message)` - Send message to current player
- `say(message)` - Say to room
- `tell_user(user, message)` - Send message to specific user
- `message()` - Advanced messaging system

### System Functions
- `call_out(function, delay, args...)` - Delayed function call
- `set_heart_beat(interval)` - Set periodic function calls
- `random(number)` - Random number generation
- `time()` - Current time

## Debugging and Development

### Common Issues
- Missing `::create()` calls in inherited objects
- Incorrect file paths (use absolute paths from `/lib/`)
- Memory leaks from objects not being destructed
- Infinite loops in heart_beat or call_out functions

### Debugging Tools
- `debug()` function for runtime information
- `/adm/bin/` utilities for testing and debugging
- Log files typically in `/lib/log/`

## Security Considerations
- Use proper security checks in sensitive functions
- Validate user input in command functions
- Respect object permissions and ownership
- Be careful with file operations and eval()

## Testing Guidelines
- Test objects by cloning them: `clone_object("/path/to/object")`
- Use `update_object()` to reload changed files
- Test in isolated environments before deploying
- Verify memory usage and cleanup

## Performance Best Practices
- Minimize heart_beat usage (CPU intensive)
- Use call_out for one-time or infrequent events
- Clean up temporary objects and variables
- Avoid deep inheritance chains
- Cache frequently accessed data

## Resources
- Lima Documentation: https://docs.limalib.dev/
- LPC Basics Manual: See `/Basic LPC manual.md`
- Intermediate LPC Manual: See `/Intermediate LPC manual.md`
- In-game help: Use `help` command for player/wizard commands

## Code Examples Location
Look in `/lib/domains/` for examples of:
- Room implementations
- NPC behaviors
- Item and equipment code
- Quest and interaction systems

When suggesting code, always consider the Lima mudlib conventions and the object-oriented nature of the LPC environment. Focus on inheritance, proper function overrides, and mudlib-specific patterns rather than generic C-style coding.
