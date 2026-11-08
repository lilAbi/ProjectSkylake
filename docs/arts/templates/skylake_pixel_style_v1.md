PROJECTSKYLAKE PIXEL STYLE v1

ART DIRECTION

ProjectSkylake uses a classic high-fantasy JRPG visual language expressed
through modern, carefully controlled pixel art.

The world should feel adventurous, mysterious, slightly romanticized,
and magical without becoming excessively ornate or visually noisy.

The visual identity combines:

- recognizable fantasy class archetypes
- elegant fantasy clothing and armor
- prominent magical, crystal, rune, relic, and elemental motifs
- practical medieval materials mixed with stylized fantasy details
- strong character silhouettes
- expressive but restrained character designs
- equipment large enough to identify immediately
- a balance between heroic fantasy and grounded adventuring gear

The overall feeling should evoke a classic fantasy RPG party exploring
an increasingly strange magical world.

Do not copy existing characters, costumes, weapons, logos, or symbols
from existing games.


PIXEL-ART LANGUAGE

Use deliberate hard-edged pixel art.

- no anti-aliasing
- no blurred edges
- no painterly brushwork
- no smooth vector-style curves
- no soft gradients
- no photographic rendering
- no realistic high-frequency texture noise

Pixels should appear intentionally placed.

Use clustered shading rather than scattered individual noise pixels.

Maintain consistent pixel density throughout the asset.


OUTLINES

Use dark colored outlines rather than pure black wherever practical.

Character and item silhouettes should normally use approximately
1 native pixel of outline thickness.

Interior outlines may be lighter than the external silhouette.

Avoid excessive internal outlining that makes small sprites difficult
to read.


SHADING

Primary lighting direction:
upper-left/front-left.

Use approximately three major values per material:

1. shadow
2. base
3. highlight

A fourth highlight may be used sparingly for:

- polished metal
- crystals
- magical materials
- glass
- strong spell effects

Avoid smooth gradients.

Use clean clusters of pixels to describe form.


COLOR

Use a controlled fantasy palette.

Base materials should be relatively grounded:

- steel
- iron
- leather
- cloth
- wood
- stone
- bone

Magical or important elements may use more saturated accent colors.

Examples:

- cyan / blue for arcane energy
- violet for mysterious magic
- red / orange for fire
- green for poison or nature
- gold for sacred or prestigious elements

Avoid having every part of an asset equally saturated.

Use one dominant color family, one secondary family, and one strong
accent where appropriate.


CHARACTER DESIGN

Characters use semi-chibi RPG proportions.

The head is slightly enlarged for readability but the body should still
support recognizable armor, weapons, and class identity.

Approximate visual proportion:

head: 1
torso: 1.5
legs: 1.5

Characters should feel heroic rather than childish.

Faces should remain simple at gameplay resolution.

Use silhouette as the primary way to distinguish classes.


CLASS SILHOUETTES

Heavy melee classes:
- broad shoulders
- large weapons
- shields
- heavy armor
- wide stance

Agile classes:
- narrow silhouette
- asymmetric equipment
- lighter armor
- smaller or paired weapons

Magic classes:
- robes
- staffs
- books
- crystals
- flowing cloth
- vertical silhouettes

Support / sacred classes:
- medium armor or robes
- staffs, maces, talismans
- strong symbolic shapes

Every class should remain identifiable when viewed only as a silhouette.


FANTASY DETAIL

Equipment may include:

- small runes
- engraved metal
- crystal inserts
- cloth banners
- belts
- buckles
- simple ornamental trim
- magical glow
- unusual fantasy alloys

Avoid excessive micro-detail that disappears at native sprite size.

Prioritize large readable shapes.


CHARACTER SPRITE STANDARD

Default gameplay character master frame:

64x64 pixels

Character should:

- fit fully inside the frame
- maintain consistent scale between characters
- use a consistent bottom-center foot pivot
- have transparent background
- have no environment
- have no baked-in floor shadow unless explicitly requested

Primary camera perspective:

slight three-quarter tactical RPG perspective.

Canonical facings:

- front
- back
- left
- right

Side sprites may be mirrored only when equipment and costume symmetry
make that visually valid.


TACTICAL READABILITY

The sprite must remain readable:

- at native resolution
- when enlarged with nearest-neighbor filtering
- inside a 2D management interface
- when displayed as a billboarded or Sprite3D-style character in a 3D
  tactical battlefield

Large silhouette information takes priority over tiny decorative detail.


MAGIC

Magic should appear visually energetic but still pixel-controlled.

Use:

- glowing crystals
- runes
- small particle clusters
- elemental shapes
- magical circles
- short energy trails

Magical effects should not obscure the character's primary silhouette
unless intentionally part of an attack animation.


MOOD

The setting should communicate:

adventure
mystery
danger
discovery
fantasy
heroism
ancient magic

Avoid:

modern technology unless specifically requested
science-fiction styling
photorealism
cartoon comedy styling
overly grim horror aesthetics
extreme anime facial exaggeration


CONSISTENCY RULE

When generating a new asset, preserve:

- pixel density
- lighting direction
- outline treatment
- shading structure
- character proportions
- camera perspective
- palette philosophy
- fantasy design language

Only change the asset-specific features requested.