The chat transcript export utility has generated the following files in this directory:

- chat_transcript.html — Formatted HTML transcript (always generated)
- chat_transcript.pdf — PDF transcript (generated only if a local HTML-to-PDF engine is available)
- chat_transcript.txt — Plain text fallback (only when PDF generation is not available)

To regenerate or update the transcript, edit utils/chat_transcript_source.txt and run:
    python3 utils/export_chat_to_pdf.py
