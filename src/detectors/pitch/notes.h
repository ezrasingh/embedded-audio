#ifndef PITCH_NOTES_H
#define PITCH_NOTES_H

#define NOTES(index)                         \
    (index == 0 ? "A" : index == 1 ? "A#/Bb" \
                    : index == 2   ? "B"     \
                    : index == 3   ? "C"     \
                    : index == 4   ? "C#/Db" \
                    : index == 5   ? "D"     \
                    : index == 6   ? "D#/Eb" \
                    : index == 7   ? "E"     \
                    : index == 8   ? "F"     \
                    : index == 9   ? "F#/Gb" \
                    : index == 10  ? "G"     \
                    : index == 11  ? "G#/Ab" \
                                   : "")

#endif // PITCH_NOTES_H