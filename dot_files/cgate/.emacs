;; .emacs

;;; uncomment this line to disable loading of "default.el" at startup
;; (setq inhibit-default-init t)

;; turn on font-lock mode
(global-font-lock-mode t)

;; enable visual feedback on selections
(setq transient-mark-mode t)

(custom-set-variables
  ;; custom-set-variables was added by Custom -- don't edit or cut/paste it!
  ;; Your init file should contain only one such instance.
 '(frame-background-mode (quote dark)))
(custom-set-faces
  ;; custom-set-faces was added by Custom -- don't edit or cut/paste it!
  ;; Your init file should contain only one such instance.
 '(default ((t (:stipple nil :background "#000000" :foreground "light green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight semi-bold :height 100 :width normal :family "adobe-courier")))))

;;three spaces as indent for C/C++
(setq c-basic-offset 3)

;;make backspace the backward delete
(global-set-key "\C-h" 'backward-delete-char) 
