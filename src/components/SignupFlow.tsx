import { useState, useEffect } from 'react';
import { CarouselPanel } from './CarouselPanel';
import { SignupForm } from './SignupForm';

export function SignupFlow() {
  return (
    <div className="flex h-screen overflow-hidden">
      {/* Left Panel - Carousel */}
      <CarouselPanel />
      
      {/* Right Panel - Signup Form */}
      <SignupForm />
    </div>
  );
}
