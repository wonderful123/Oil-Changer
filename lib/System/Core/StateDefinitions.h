#pragma once

/**
 * @file StateDefinitions.h
 * @brief This file contains the definitions of various states used in the
 * system. These states are utilized by components implementing TinyFSM to
 * manage their internal state machines.
 *
 * @details
 * - The states defined here should be broad enough to be applicable across
 *   different components, yet specific enough to provide meaningful context.
 * - Enumerations are used for defining states, ensuring type safety and
 * clarity.
 * - Modify or extend these definitions as the system evolves and new states are
 * needed.
 */

// Example State Enumeration for a component
enum class SystemState { Initialization, Idle, Running, Error, Shutdown };

// Add additional state enumerations as needed for other components
