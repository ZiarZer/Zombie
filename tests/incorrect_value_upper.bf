Initially: A = 0 and B = 0

++++           Increment A of 4
[              while A != 0
    -              Decremente A of 1
    >++++<         Increment B of 4
]              end of loop: A = 0 and B = 16
>
[              while B != 0:
    -              Decrement B of 1
    <++++>         Increment A of 4
]              end of loop: A = 64 and B = 0
<
[              while A != 0:
    -              Decrement A of 1
    >++++<         Increment B of 4
]              end of loop: A = 0 and B = 256

Error on the very last increment because B = 255 cannot be incremented
