#!/usr/bin/env python3
import json
import sys
import argparse
from typing import Dict, List, Tuple, Optional, Set

def char_to_num(c: str) -> int:
    """Convert character to number (A=0, B=1, ..., Z=25)"""
    return ord(c.upper()) - ord('A')

def num_to_char(n: int) -> str:
    """Convert number to character (0=A, 1=B, ..., 25=Z)"""
    return chr((n % 26) + ord('A'))

def class_func(i: int) -> int:
    """Compute class for index i"""
    return ((i % 2) * 3) + (i % 3)

def compute_residue(family: str, c_val: int, p_val: int) -> int:
    """
    Compute residue K based on family rule (mod 26):
    Vigenere: P = C - K => K = C - P
    Beaufort: P = K - C => K = P + C  
    Variant-Beaufort: P = C + K => K = P - C
    """
    if family == "vigenere":
        return (c_val - p_val) % 26
    elif family == "beaufort":
        return (p_val + c_val) % 26
    elif family == "variant_beaufort":
        return (p_val - c_val) % 26
    else:
        raise ValueError(f"Unknown family: {family}")

def decode_with_residue(family: str, c_val: int, k_val: int) -> int:
    """
    Decode ciphertext using residue K based on family rule (mod 26):
    Vigenere: P = C - K
    Beaufort: P = K - C
    Variant-Beaufort: P = C + K
    """
    if family == "vigenere":
        return (c_val - k_val) % 26
    elif family == "beaufort":
        return (k_val - c_val) % 26
    elif family == "variant_beaufort":
        return (c_val + k_val) % 26
    else:
        raise ValueError(f"Unknown family: {family}")

def is_additive_family(family: str) -> bool:
    """Check if family is additive (Vigenere or Variant-Beaufort)"""
    return family in ["vigenere", "variant_beaufort"]

def try_assignment(class_num: int, anchors_for_class: List[Tuple[int, int, int]], 
                   families: List[str], periods: range, max_phase: int) -> Optional[Tuple[str, int, int, Dict[int, int]]]:
    """
    Try to find a valid (family, L, phase) assignment for a class.
    Returns (family, L, phase, residues_dict) if found, None otherwise.
    """
    for family in families:
        is_additive = is_additive_family(family)
        
        for L in periods:
            for phase in range(min(L, max_phase + 1)):
                # Check for collisions
                slots_to_residue = {}
                valid = True
                
                for idx, c_val, p_val in anchors_for_class:
                    slot = (idx + phase) % L
                    k_val = compute_residue(family, c_val, p_val)
                    
                    # Option-A constraint: additive families need K != 0 at anchors
                    if is_additive and k_val == 0:
                        valid = False
                        break
                    
                    if slot in slots_to_residue:
                        if slots_to_residue[slot] != k_val:
                            valid = False
                            break
                    else:
                        slots_to_residue[slot] = k_val
                
                if valid:
                    return family, L, phase, slots_to_residue
    
    return None

def main():
    parser = argparse.ArgumentParser(description='Reconstruct class wheels from ciphertext and crib letters')
    parser.add_argument('--ct', required=True, help='Ciphertext file')
    parser.add_argument('--anchors', required=True, help='JSON file with anchor indices and plaintext letters')
    args = parser.parse_args()
    
    # Read ciphertext
    with open(args.ct, 'r') as f:
        ciphertext = f.read().strip().upper()
    
    # Read anchors
    with open(args.anchors, 'r') as f:
        anchors_data = json.load(f)
    
    # Process anchors
    anchors = {}  # index -> plaintext char
    for item in anchors_data:
        idx = item['index']
        letter = item['letter'].upper()
        anchors[idx] = letter
    
    # Group anchors by class
    class_anchors = {i: [] for i in range(6)}
    for idx, p_char in anchors.items():
        class_num = class_func(idx)
        c_char = ciphertext[idx]
        c_val = char_to_num(c_char)
        p_val = char_to_num(p_char)
        class_anchors[class_num].append((idx, c_val, p_val))
    
    # Try to assign family, L, phase for each class
    families = ["vigenere", "beaufort", "variant_beaufort"]
    periods = range(10, 23)  # [10..22]
    
    class_assignments = {}
    
    for class_num in range(6):
        if not class_anchors[class_num]:
            # No anchors for this class, assign default values
            class_assignments[class_num] = {
                "family": "vigenere",
                "L": 10,
                "phase": 0,
                "residues": {}
            }
        else:
            result = try_assignment(class_num, class_anchors[class_num], families, periods, 22)
            if result:
                family, L, phase, residues = result
                class_assignments[class_num] = {
                    "family": family,
                    "L": L,
                    "phase": phase,
                    "residues": {str(k): v for k, v in residues.items()}
                }
            else:
                # Failed to find valid assignment
                print(f"Error: Could not find valid assignment for class {class_num}")
                sys.exit(1)
    
    # Derive plaintext
    plaintext = ['?'] * len(ciphertext)
    determined_indices = set()
    
    for idx in range(len(ciphertext)):
        class_num = class_func(idx)
        assignment = class_assignments[class_num]
        
        family = assignment["family"]
        L = assignment["L"]
        phase = assignment["phase"]
        residues = assignment["residues"]
        
        slot = (idx + phase) % L
        
        if str(slot) in residues:
            k_val = residues[str(slot)]
            c_val = char_to_num(ciphertext[idx])
            p_val = decode_with_residue(family, c_val, k_val)
            plaintext[idx] = num_to_char(p_val)
            determined_indices.add(idx)
    
    plaintext_str = ''.join(plaintext)
    
    # Get undetermined indices
    undetermined_indices = [i for i in range(len(ciphertext)) if i not in determined_indices]
    
    # Prepare output JSON
    output_data = {
        "classes": {}
    }
    
    for class_num in range(6):
        assignment = class_assignments[class_num]
        output_data["classes"][str(class_num)] = {
            "family": assignment["family"],
            "L": assignment["L"],
            "phase": assignment["phase"],
            "residues": assignment["residues"]
        }
    
    output_data["undetermined_indices"] = undetermined_indices
    
    # Write outputs
    with open('wheel_reconstruction.json', 'w') as f:
        json.dump(output_data, f, indent=2)
    
    with open('derived_pt.txt', 'w') as f:
        f.write(plaintext_str)
    
    print(f"Reconstruction complete. Determined {len(determined_indices)} of {len(ciphertext)} positions.")
    print(f"Outputs: wheel_reconstruction.json, derived_pt.txt")

if __name__ == "__main__":
    main()