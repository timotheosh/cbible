;;; cbible.el --- SWORD Bible lookup and commentary integration -*- lexical-binding: t; -*-

;; Copyright (C) 2015-2026 Tim Hawes
;; Version: 0.21
;; Keywords: tools, bible

;;; Commentary:
;; A minor mode for inserting Scripture text and writing Personal commentary
;; through the cbible executable.  No command is evaluated by a shell.

;;; Code:

(require 'subr-x)

(defgroup cbible nil
  "Use the cbible command from Emacs."
  :group 'tools
  :prefix "cbible-")

(defcustom cbible-program "cbible"
  "Path to the cbible executable."
  :type 'file
  :group 'cbible)

(defcustom cbible-bible-version "KJV"
  "Default SWORD Bible module."
  :type 'string
  :group 'cbible)

(define-obsolete-variable-alias 'bibleversion 'cbible-bible-version "0.20")

(defun cbible--run (&rest arguments)
  "Run cbible with ARGUMENTS and return its standard output."
  (with-temp-buffer
    (let ((status (apply #'process-file cbible-program nil t nil arguments)))
      (unless (zerop status)
        (error "cbible failed: %s" (string-trim-right (buffer-string))))
      (string-trim-right (buffer-string)))))

(defun cbible-reference (reference &optional version)
  "Return REFERENCE from VERSION or `cbible-bible-version'."
  (cbible--run "-b" (if (and version (not (string-empty-p version)))
                         version
                       cbible-bible-version)
               "-r" reference))

(defun cbible-lookup ()
  "Prompt for a Scripture reference and module, then insert its text."
  (interactive)
  (let* ((version (read-string "Bible version: " cbible-bible-version))
         (reference (read-string "Reference: ")))
    (insert (cbible-reference reference version))))

(defun cbible--write-entry (text reference)
  "Write TEXT to Personal commentary at REFERENCE."
  (with-temp-buffer
    (insert text)
    (let ((status (call-process-region (point-min) (point-max)
                                       cbible-program nil t nil
                                       "-b" "Personal" "-r" reference "-i")))
      (unless (zerop status)
        (error "cbible failed: %s" (string-trim-right (buffer-string)))))))

(defun cbible-make-entry (entry)
  "Prompt for a reference and write ENTRY to Personal commentary."
  (cbible--write-entry entry (read-string "Reference: ")))

(defun cbible-entry-region (start end)
  "Send the region between START and END to Personal commentary."
  (interactive "r")
  (cbible-make-entry (buffer-substring-no-properties start end)))

(defun cbible-entry-buffer ()
  "Send the current buffer to Personal commentary."
  (interactive)
  (cbible-make-entry (buffer-substring-no-properties (point-min) (point-max))))

(defvar cbible-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map (kbd "C-c l") #'cbible-lookup)
    map)
  "Keymap for `cbible-mode'.")

(define-minor-mode cbible-mode
  "Insert Bible passages and save commentary through cbible."
  :lighter " cbible"
  :keymap cbible-mode-map
  :group 'cbible)

(provide 'cbible)
;;; cbible.el ends here
