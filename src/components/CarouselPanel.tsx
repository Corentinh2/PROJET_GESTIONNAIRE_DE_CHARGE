import { useState, useEffect } from 'react';
import { motion, AnimatePresence } from 'motion/react';
import { ImageWithFallback } from './figma/ImageWithFallback';

const carouselSlides = [
  {
    image: 'https://images.unsplash.com/photo-1735639013995-086e648eaa38?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHx0ZWFtJTIwY29sbGFib3JhdGlvbiUyMHdvcmtzcGFjZXxlbnwxfHx8fDE3Njg0MTk0MTB8MA&ixlib=rb-4.1.0&q=80&w=1080',
    title: 'Collaborez en équipe',
    description: 'Travaillez ensemble de manière fluide et efficace avec vos collègues.'
  },
  {
    image: 'https://images.unsplash.com/photo-1762341121807-c0a69eb8a92c?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHxtb2Rlcm4lMjBvZmZpY2UlMjB0ZWNobm9sb2d5fGVufDF8fHx8MTc2ODQ1OTI4OHww&ixlib=rb-4.1.0&q=80&w=1080',
    title: 'Technologie moderne',
    description: 'Profitez des outils les plus avancés pour optimiser votre productivité.'
  },
  {
    image: 'https://images.unsplash.com/photo-1626105985445-6430a31f6f96?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHxzdWNjZXNzZnVsJTIwYnVzaW5lc3MlMjBtZWV0aW5nfGVufDF8fHx8MTc2ODQ4Mzk5OHww&ixlib=rb-4.1.0&q=80&w=1080',
    title: 'Réussissez ensemble',
    description: 'Atteignez vos objectifs grâce à une collaboration intelligente.'
  },
  {
    image: 'https://images.unsplash.com/photo-1523961131990-5ea7c61b2107?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHxkaWdpdGFsJTIwaW5ub3ZhdGlvbnxlbnwxfHx8fDE3Njg0MzY2MzN8MA&ixlib=rb-4.1.0&q=80&w=1080',
    title: 'Innovation digitale',
    description: 'Restez à la pointe de l\'innovation avec notre plateforme.'
  }
];

export function CarouselPanel() {
  const [currentSlide, setCurrentSlide] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => {
      setCurrentSlide((prev) => (prev + 1) % carouselSlides.length);
    }, 5000); // Change every 5 seconds

    return () => clearInterval(interval);
  }, []);

  return (
    <div className="hidden lg:flex lg:w-1/2 bg-gradient-to-br from-indigo-600 via-purple-600 to-pink-500 relative overflow-hidden">
      {/* Carousel Images */}
      <AnimatePresence mode="wait">
        <motion.div
          key={currentSlide}
          initial={{ opacity: 0, scale: 1.1 }}
          animate={{ opacity: 1, scale: 1 }}
          exit={{ opacity: 0, scale: 0.95 }}
          transition={{ duration: 0.7, ease: 'easeInOut' }}
          className="absolute inset-0"
        >
          <div className="absolute inset-0 bg-black/40 z-10" />
          <ImageWithFallback
            src={carouselSlides[currentSlide].image}
            alt={carouselSlides[currentSlide].title}
            className="w-full h-full object-cover"
          />
        </motion.div>
      </AnimatePresence>

      {/* Content Overlay */}
      <div className="relative z-20 flex flex-col justify-between p-12 text-white w-full">
        {/* Logo/Brand */}
        <div>
          <h1 className="text-3xl font-bold">VotreMarque</h1>
        </div>

        {/* Centered Content */}
        <div className="space-y-6">
          <AnimatePresence mode="wait">
            <motion.div
              key={currentSlide}
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              exit={{ opacity: 0, y: -20 }}
              transition={{ duration: 0.5, delay: 0.2 }}
            >
              <h2 className="text-5xl font-bold mb-4">
                {carouselSlides[currentSlide].title}
              </h2>
              <p className="text-xl text-white/90 max-w-md">
                {carouselSlides[currentSlide].description}
              </p>
            </motion.div>
          </AnimatePresence>
        </div>

        {/* Carousel Indicators */}
        <div className="flex gap-2">
          {carouselSlides.map((_, index) => (
            <button
              key={index}
              onClick={() => setCurrentSlide(index)}
              className={`h-1 rounded-full transition-all duration-300 ${
                index === currentSlide ? 'w-12 bg-white' : 'w-8 bg-white/40'
              }`}
              aria-label={`Go to slide ${index + 1}`}
            />
          ))}
        </div>
      </div>
    </div>
  );
}
