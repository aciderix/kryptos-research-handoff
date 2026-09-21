# K4 Documentary Research — 2026-09-21

**Phase:** Documentary search only
**Execution policy:** No K4 calculation was run. No new experimental hypothesis was created from these findings.

## Conclusion

The documentary search found several independent facts that narrow the type of K4 mechanism, but none currently fixes a complete, novel plaintext-discovery recipe. The strongest facts are that the lower text uses **two systems of enciphering**, that Scheidt described K4 as a process that **masks the English language**, and that Sanborn later described the solution as a method rather than merely a recovered text. These facts constrain a mechanism class, but they do not specify the representation, position map, parameters, and final transformation.

The current state therefore remains:

> **No new admissible experiment identified.**

## 1. Two systems of enciphering the bottom text

### Fait source

A transcript of Sanborn’s dedication remarks, reproduced in the archive research collection, attributes to Sanborn the statement:

> “There are two systems of enciphering the bottom text. ... yes, there are two separate systems and that is a major clue in itself.” [4]

The CIA’s official description independently states that the first three texts used the Vigenère tableau together with matrix coding systems, while K4 was designed as a more difficult fourth section. [1]

### Interprétation

The lower physical text should not automatically be modeled as one monolithic cipher. “Two systems” is stronger than a generic suggestion that K4 is complicated. It indicates an ordered composition or a split mechanism, but the wording does not establish whether the systems are serial, spatially divided, masking-plus-cipher, or a cipher combined with a transposition.

### Mécanisme contraint

The fact constrains the model to a composition of two systems for the bottom text. It does **not** determine their order, their boundaries, their inputs, or whether one system operates on positions rather than symbols.

### Recette

No complete recipe follows. The repository already contains a two-system landscape and several families involving transposition, masking, physical overlays, and substitution. This discovery is therefore **prior constraint / OPEN CELL**, not a new experiment. A future recipe would need an independent source fixing the two operations and their order.

## 2. “I masked the English language”

### Fait source

In a 2005 interview, Ed Scheidt said that the first three processes left English accessible to cryptanalysis, while in the fourth process “I masked the English language.” He added that the masking technique “may not be known” and that it was part of the puzzle he had agreed to keep secret. [2]

The same interview records uncertainty about changes Sanborn may have made after Scheidt’s involvement. Scheidt said he knew what they had discussed but did not know what Sanborn ultimately put into the sculpture.

### Interprétation

This is direct evidence for a **masking layer or concealment operation**, not merely a difficult substitution. It supports families in which the readable English stream is hidden among, reordered with, or structurally combined with other material.

However, “mask” is not an algorithm. It does not distinguish a null mask, Cardan grille, transposition, physical overlay, decoy text, or another operation. The interview also warns that the consultant may not know Sanborn’s final implementation.

### Mécanisme contraint

At most, the fact constrains K4 to include a mechanism that reduces ordinary English visibility before or during encryption. It does not fix the mask positions, the mask source, the geometry, or the inverse operation.

### Recette

No deterministic recipe is forced. The internal registry already contains Morse masks, physical overlays, null-mask models, and two-system constructions. The fact is therefore **AUDITED PRIOR ART / OPEN CELL**, not a new experiment.

## 3. “The method is the real deal”

### Fait source

In reporting on the 2025 archive discovery, Sanborn distinguished recovering the plaintext from deciphering K4: “They discovered it. They did not decipher it. They do not have the key. They do not have the method with which it’s deciphered.” [3]

### Interprétation

This confirms the project’s evidentiary distinction between a candidate plaintext and a forward derivation. It also indicates that the archive contains a procedure or key material that Sanborn regards as essential to a solution.

### Mécanisme contraint

The fact constrains the validation standard: a plaintext without the method is incomplete. It does not reveal the method and therefore cannot determine a new recipe.

### Recette

None. This is a **validation requirement**, not a mechanism hypothesis.

## 4. The 1986 Egypt trip and the 1989 Berlin Wall event

### Fait source

Scientific American reports four clues announced by Sanborn in 2025: the 1986 trip to Egypt, the 1989 fall of the Berlin Wall, the interpretation of `BERLINCLOCK` as Berlin’s World Clock, and the theme of “delivering a message.” [5]

### Interprétation

These facts constrain likely plaintext subject matter and historical reference. The World Clock clarification is materially useful for interpreting the known crib, but it does not specify an encryption operation. The Egypt and Berlin references may provide plaintext content or a keying context, but the source does not state how they enter the cipher.

### Mécanisme contraint

No operation is forced. The clues constrain semantics, not representation or transformation.

### Recette

No recipe. Status: **FACTUAL CLUES / OPEN CELL**.

## 5. Physical construction and orientation

### Fait source

The CIA describes an S-shaped copper screen, a separate left encoded-text side, and a right-side Vigenère tableau. It states that the tableau was intentionally flipped so it could be read from behind. The CIA also records the use of matrix coding systems for the first three sections and says Sanborn worked for four months with a retired CIA cryptographer. [1]

The CIA’s transcription presents K4 as the final 97-character block beginning with `OBKR`. [1]

### Interprétation

The physical orientation is a real design constraint for the first three systems and may matter to the installation as a whole. It does not prove that K4 is decrypted by looking through the panels, reading from behind, or mirroring the tableau. Those are separate hypotheses.

### Mécanisme contraint

The fact fixes the existence and orientation of the physical tableau, but not a K4 position map or a new operation. Full-panel overlays, mirrored readings, and tableau reflows are already represented in the internal registry.

### Recette

No new recipe. Status: **PUBLIC FACT / ALREADY COVERED OR OPEN CELL**, depending on the proposed use.

## 6. Archive-research items requiring source-level authentication

The curated archive page reports several potentially important items: a handwritten line that an encrypted message is included within a larger set of characters and could be used “to shade an area”; a “Code Breaker” overlay sketch; a job order reading `7×88 on a side`; and fabrication documents involving mirror-image text. [4]

These are more promising than generic thematic clues because they could constrain a masking or physical-overlay mechanism. Nevertheless, the current evidence is a curator’s description of archive images, not yet a verified transcription and provenance audit of each original document.

### Fait source

The archive page attributes these descriptions to specific image identifiers and distinguishes some items as uncertain. In particular, it says that the “Code Breaker” overlay may not reflect Kryptos, and that one earlier reading of “Overlay” was corrected to “Overlord.” [4]

### Interprétation

The material suggests that Sanborn thought in terms of hidden content, shaded regions, templates, and physical layers. The `7×88` job order could be a manufacturing constraint, but its relationship to K4 is not established by the page alone. Mirror-image waterjet work may concern another sculpture or a later fabrication job.

### Mécanisme contraint

No complete mechanism is currently forced. The evidence could become decisive only after inspecting the original images, confirming dates and project identity, and establishing that a document refers specifically to K4 rather than to another Sanborn work or a general design study.

### Recette

No execution. Status: **PROMISING DOCUMENTARY LEAD / NOT YET AUTHENTICATED**.

## 7. Historical relation to K3

### Fait source

A public K3 reconstruction describes a double transposition with widths 21 and 28, while noting uncertainty about the question mark between K3 and K4. The same source reports an attributed remark that Sanborn was surprised no one had tried recovering the original matrix and running it through possible shifts. [6]

The CIA and other historical accounts establish that K1–K3 were intended to expose English to ordinary cryptanalysis, while K4 was designed to hide that accessibility. [1] [2]

### Interprétation

K3 demonstrates that physical line structure and matrix operations can be deliberate. The remark about recovering the original matrix, if independently authenticated, could constrain a K4 transfer procedure. The public K3 page itself is not a primary transcript, and its K4 implications are analogical.

### Mécanisme contraint

The known K3 method constrains a family already registered in the project. It does not force its reuse on K4, nor does it determine a K4 geometry or key.

### Recette

No new experiment. Status: **DOCUMENTED K3 FACT / K4 TRANSFER OPEN CELL**.

## Filter results

| Discovery | Determination filter | Novelty filter | Status |
|---|---|---|---|
| Two systems of enciphering | Fails: operations and order unspecified | Already represented by two-system families | Prior constraint / OPEN CELL |
| Masked English language | Fails: mask type and inverse unspecified | Mask and overlay families already represented | Audited prior art / OPEN CELL |
| Method distinct from plaintext | Does not specify a mechanism | Reinforces existing evidence policy | Validation requirement |
| Egypt, Berlin Wall, World Clock, delivering a message | Semantically constraining only | No new transformation | Factual clues / OPEN CELL |
| Flipped tableau and physical layout | Orientation is fixed, K4 use is not | Overlay and mirror families already covered | Public fact / no new recipe |
| Archive “shade an area,” overlay sketch, `7×88` | Source-level identity and mapping unresolved | Could be novel if authenticated | Documentary lead / not admissible |
| K3 matrix and possible shifts | K4 transfer not fixed | K3 families already audited | K4 transfer / OPEN CELL |

## Final research state

No source reviewed in this phase supplies all of the following at once: an exact K4 input representation, a fixed alignment, a unique position map, fixed parameters, a specified operation, and a determined inverse transformation. Therefore no new calculation is justified.

The next useful work is documentary authentication of the archive items that refer to shading, templates, `7×88`, and fabrication layout. That work should remain source criticism, not become a parameter search.

## References

[1]: https://www.cia.gov/legacy/headquarters/kryptos-sculpture/ "CIA, ‘Kryptos’ Sculpture"
[2]: https://www.wired.com/2005/01/inside-info-on-kryptos-codes/ "Wired, ‘Inside Info on Kryptos’ Codes’"
[3]: https://apnews.com/article/kryptos-jim-sanborn-auction-cia-650c1253d6a96591f29b88a20299c430 "Associated Press, ‘Kryptos’ final code remains unsolved’"
[4]: https://www.kryptosbot.com/archive/ "KryptosBot, Sanborn Papers — Archive Research Collection"
[5]: https://www.scientificamerican.com/article/cia-kryptos-puzzle-creator-releases-final-clues/ "Scientific American, ‘CIA Kryptos Puzzle Creator Releases Final Clues’"
[6]: https://rumkin.com/reference/kryptos/k3/ "Rumkin, ‘K3 — Two Transposition’"
