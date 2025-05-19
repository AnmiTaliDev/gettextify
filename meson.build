project('csv-to-mo', 'cs',
    version: '1.0.0',
    default_options: ['warning_level=2'])

# Define sources
src = [
    'src/Program.cs',
    'src/Translate.cs'
]

# Build executable
executable('csv-to-mo', 
    sources: src,
    install: true,
    cs_args: ['-optimize']
)